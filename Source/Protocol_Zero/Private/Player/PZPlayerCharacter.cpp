// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/PZPlayerCharacter.h"
#include "Player/PZAimMath.h"
#include "Player/PZMovementMath.h"
#include "Combat/PZHealthComponent.h"
#include "Combat/PZWeaponComponent.h"
#include "Combat/PZWeaponData.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

APZPlayerCharacter::APZPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Facing is driven by aim, never by the controller or by movement.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->SetRelativeScale3D(FVector(0.7f, 0.7f, 1.8f));
	if (CylinderMesh.Succeeded())
	{
		Body->SetStaticMesh(CylinderMesh.Object);
	}

	FacingMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FacingMarker"));
	FacingMarker->SetupAttachment(RootComponent);
	FacingMarker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FacingMarker->SetRelativeLocation(FVector(45.f, 0.f, 30.f));
	FacingMarker->SetRelativeScale3D(FVector(0.4f, 0.2f, 0.2f));
	if (CubeMesh.Succeeded())
	{
		FacingMarker->SetStaticMesh(CubeMesh.Object);
	}

	Health = CreateDefaultSubobject<UPZHealthComponent>(TEXT("Health"));
	// Placeholder tuning: a short grace window so stacked drone hits cannot kill instantly.
	Health->SetTuning(100.f, 0.5f);

	static ConstructorHelpers::FObjectFinder<UPZWeaponData> PlasmaRifle(TEXT("/Game/Data/Weapons/DA_PlasmaRifle.DA_PlasmaRifle"));
	Weapon = CreateDefaultSubobject<UPZWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
	Weapon->SetRelativeLocation(FVector(70.f, 0.f, 30.f));
	if (PlasmaRifle.Succeeded())
	{
		Weapon->SetWeaponData(PlasmaRifle.Object);
	}

	ApplyTuning();
}

void APZPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplyTuning();
}

void APZPlayerCharacter::ApplyTuning()
{
	CameraBoom->SetWorldRotation(FRotator(CameraPitch, CameraYaw, 0.f));
	CameraBoom->TargetArmLength = CameraDistance;
	Camera->SetFieldOfView(CameraFieldOfView);
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void APZPlayerCharacter::AimAt(const FVector& WorldPoint)
{
	AimPoint = WorldPoint;
	Weapon->SetAimPoint(WorldPoint);

	float Yaw = 0.f;
	if (PZAimMath::YawToward(GetActorLocation(), WorldPoint, Yaw))
	{
		SetActorRotation(FRotator(0.f, Yaw, 0.f));
	}
}

void APZPlayerCharacter::Move(const FVector2D& Input)
{
	const FVector Direction = PZMovementMath::CameraRelativeDirection(Input, CameraYaw);
	AddMovementInput(Direction, Direction.Size());
}
