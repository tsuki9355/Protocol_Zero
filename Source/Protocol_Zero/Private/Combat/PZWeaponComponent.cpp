// Copyright Epic Games, Inc. All Rights Reserved.

#include "Combat/PZWeaponComponent.h"
#include "Combat/PZProjectile.h"
#include "Combat/PZWeaponData.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

namespace
{
	constexpr float DebugShotLength = 1500.f;
	constexpr float DebugShotLifetime = 0.15f;
}

UPZWeaponComponent::UPZWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPZWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponData)
	{
		Cadence.SetCooldown(WeaponData->FireCooldown);
	}
}

double UPZWeaponComponent::GetNow() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimeSeconds() : 0.0;
}

void UPZWeaponComponent::StartFire()
{
	Cadence.PullTrigger(GetNow());
	SetComponentTickEnabled(true);
}

void UPZWeaponComponent::StopFire()
{
	Cadence.ReleaseTrigger();
	SetComponentTickEnabled(false);
}

void UPZWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const int32 Shots = Cadence.ConsumeDueShots(GetNow());
	for (int32 Index = 0; Index < Shots; ++Index)
	{
		FireShot();
	}
}

void UPZWeaponComponent::FireShot()
{
	const FVector Muzzle = GetComponentLocation();
	FVector Direction = AimPoint - Muzzle;
	Direction.Z = 0.0;
	if (!Direction.Normalize())
	{
		Direction = GetOwner() ? GetOwner()->GetActorForwardVector() : GetForwardVector();
	}

	UWorld* World = GetWorld();
	if (World && WeaponData && WeaponData->ProjectileClass)
	{
		const FTransform SpawnTransform(Direction.Rotation(), Muzzle);
		APZProjectile* Bolt = World->SpawnActorDeferred<APZProjectile>(
			WeaponData->ProjectileClass, SpawnTransform, GetOwner(), GetOwner() ? GetOwner()->GetInstigator() : nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (Bolt)
		{
			Bolt->SetDamage(WeaponData->Damage);
			Bolt->FinishSpawning(SpawnTransform);
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawDebugShots && World)
	{
		DrawDebugLine(World, Muzzle, Muzzle + Direction * DebugShotLength, FColor::Cyan, false, DebugShotLifetime, 0, 2.f);
	}
#endif

	OnFired.Broadcast(Muzzle, Direction);
}
