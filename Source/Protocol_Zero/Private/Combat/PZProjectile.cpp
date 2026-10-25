// Copyright Epic Games, Inc. All Rights Reserved.

#include "Combat/PZProjectile.h"
#include "Combat/PZHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

APZProjectile::APZProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	InitialLifeSpan = 3.f;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(8.f);
	Collision->SetCollisionProfileName(TEXT("PlayerProjectile"));
	RootComponent = Collision;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
	Visual->SetupAttachment(Collision);
	Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Visual->SetRelativeScale3D(FVector(0.3f, 0.15f, 0.15f));
	Visual->SetCastShadow(false);
	if (SphereMesh.Succeeded())
	{
		Visual->SetStaticMesh(SphereMesh.Object);
	}

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->UpdatedComponent = Collision;
	Movement->InitialSpeed = 2500.f;
	Movement->MaxSpeed = 2500.f;
	Movement->ProjectileGravityScale = 0.f;
	Movement->bShouldBounce = false;
	Movement->bRotationFollowsVelocity = true;
}

void APZProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Movement->OnProjectileStop.AddDynamic(this, &APZProjectile::HandleStop);
}

void APZProjectile::HandleStop(const FHitResult& Hit)
{
	if (AActor* HitActor = Hit.GetActor())
	{
		if (UPZHealthComponent* Health = HitActor->FindComponentByClass<UPZHealthComponent>())
		{
			const FVector CameFrom = -GetActorForwardVector();
			Health->ApplyDamage(Damage, CameFrom);
		}
	}

	Destroy();
}
