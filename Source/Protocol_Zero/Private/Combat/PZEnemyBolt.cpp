// Copyright Epic Games, Inc. All Rights Reserved.

#include "Combat/PZEnemyBolt.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

APZEnemyBolt::APZEnemyBolt()
{
	InitialLifeSpan = 4.f;

	Collision->SetCollisionProfileName(TEXT("EnemyProjectile"));
	Collision->SetSphereRadius(12.f);

	Visual->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	Movement->InitialSpeed = 1200.f;
	Movement->MaxSpeed = 1200.f;
}
