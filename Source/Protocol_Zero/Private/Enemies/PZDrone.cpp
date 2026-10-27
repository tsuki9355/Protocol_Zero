// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemies/PZDrone.h"
#include "Enemies/PZDestructionMath.h"
#include "Enemies/PZDroneData.h"
#include "Enemies/PZHunterAIController.h"
#include "Combat/PZHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PZLog.h"
#include "UObject/ConstructorHelpers.h"

APZDrone::APZDrone()
{
	// Ticks only while playing the destruction effect.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AIControllerClass = APZHunterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Match the navmesh agent radius so drones fit every gap the navmesh allows.
	GetCapsuleComponent()->InitCapsuleSize(35.f, 60.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	Body->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.5f));
	if (SphereMesh.Succeeded())
	{
		Body->SetStaticMesh(SphereMesh.Object);
	}

	Health = CreateDefaultSubobject<UPZHealthComponent>(TEXT("Health"));

	static ConstructorHelpers::FObjectFinder<UPZDroneData> HunterData(TEXT("/Game/Data/Enemies/DA_Hunter.DA_Hunter"));
	if (HunterData.Succeeded())
	{
		DroneData = HunterData.Object;
	}
}

void APZDrone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DroneData)
	{
		Health->SetTuning(DroneData->MaxHealth, 0.f);
		GetCharacterMovement()->MaxWalkSpeed = DroneData->MoveSpeed;
		Melee = FPZMeleeAttack(DroneData->AttackRange, DroneData->AttackCooldown);
	}

	Health->OnDeath.AddDynamic(this, &APZDrone::HandleDeath);
}

bool APZDrone::IsInMeleeRange(const AActor* Target) const
{
	return Target && Melee.IsInRange(FVector::Dist2D(GetActorLocation(), Target->GetActorLocation()));
}

bool APZDrone::TryMeleeAttack(AActor* Target)
{
	if (!Target || Health->IsDead())
	{
		return false;
	}

	const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;
	if (!Melee.TryAttack(Now, FVector::Dist2D(GetActorLocation(), Target->GetActorLocation())))
	{
		return false;
	}

	if (UPZHealthComponent* TargetHealth = Target->FindComponentByClass<UPZHealthComponent>())
	{
		const float Damage = DroneData ? DroneData->AttackDamage : 0.f;
		const FVector CameFrom = (GetActorLocation() - Target->GetActorLocation()).GetSafeNormal2D();
		TargetHealth->ApplyDamage(Damage, CameFrom);
		UE_LOG(LogProtocolZero, Log, TEXT("%s melee attack on %s for %.1f (target health now %.1f)"),
			*GetName(), *Target->GetName(), Damage, TargetHealth->GetCurrentHealth());
	}
	return true;
}

void APZDrone::HandleDeath()
{
	bDying = true;
	DyingElapsed = 0.f;
	BodyBaseScale = Body->GetRelativeScale3D();

	// Out of the fight at once: no more collisions, movement or AI while the effect plays.
	SetActorEnableCollision(false);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	DetachFromControllerPendingDestroy();

	UE_LOG(LogProtocolZero, Log, TEXT("%s destroyed"), *GetName());
	SetActorTickEnabled(true);
}

void APZDrone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bDying)
	{
		return;
	}

	const float Duration = DroneData ? DroneData->DestructionDuration : 0.f;
	const float SpinRate = DroneData ? DroneData->DestructionSpinRate : 0.f;
	DyingElapsed += DeltaSeconds;

	Body->SetRelativeScale3D(BodyBaseScale * PZDestructionMath::DestructionScale(DyingElapsed, Duration));
	Body->AddLocalRotation(FRotator(0.f, SpinRate * DeltaSeconds, 0.f));

	if (DyingElapsed >= Duration)
	{
		UE_LOG(LogProtocolZero, Log, TEXT("%s despawned after %.2fs destruction effect"), *GetName(), DyingElapsed);
		Destroy();
	}
}
