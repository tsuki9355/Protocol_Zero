// Copyright Epic Games, Inc. All Rights Reserved.

#include "Combat/PZHealthState.h"

FPZHealthState::FPZHealthState(float InMaxHealth, float InInvulnerabilityDuration)
	: MaxHealth(FMath::Max(InMaxHealth, 1.f))
	, InvulnerabilityDuration(FMath::Max(InInvulnerabilityDuration, 0.f))
{
	Reset();
}

EPZDamageResult FPZHealthState::ApplyDamage(float Amount, double Now)
{
	if (Amount <= 0.f || IsDead() || IsInvulnerable(Now))
	{
		return EPZDamageResult::Ignored;
	}

	CurrentHealth = FMath::Max(CurrentHealth - Amount, 0.f);
	InvulnerableUntil = Now + InvulnerabilityDuration;
	return IsDead() ? EPZDamageResult::Killed : EPZDamageResult::Damaged;
}

void FPZHealthState::Reset()
{
	CurrentHealth = MaxHealth;
	InvulnerableUntil = TNumericLimits<double>::Lowest();
}
