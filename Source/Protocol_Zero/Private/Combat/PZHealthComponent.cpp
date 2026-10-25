// Copyright Epic Games, Inc. All Rights Reserved.

#include "Combat/PZHealthComponent.h"
#include "Engine/World.h"

UPZHealthComponent::UPZHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPZHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetHealth();
}

void UPZHealthComponent::ResetHealth()
{
	State = FPZHealthState(MaxHealth, InvulnerabilityDuration);
}

void UPZHealthComponent::ApplyDamage(float Amount, FVector HitDirection)
{
	const UWorld* World = GetWorld();
	const double Now = World ? World->GetTimeSeconds() : 0.0;
	const float HealthBefore = State.GetCurrentHealth();

	const EPZDamageResult Result = State.ApplyDamage(Amount, Now);
	if (Result == EPZDamageResult::Ignored)
	{
		return;
	}

	OnDamaged.Broadcast(HealthBefore - State.GetCurrentHealth(), HitDirection, State.GetCurrentHealth());
	if (Result == EPZDamageResult::Killed)
	{
		OnDeath.Broadcast();
	}
}
