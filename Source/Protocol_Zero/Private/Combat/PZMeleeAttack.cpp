// Copyright Epic Games, Inc. All Rights Reserved.

#include "Combat/PZMeleeAttack.h"

FPZMeleeAttack::FPZMeleeAttack(float InRange, float InCooldown)
	: Range(FMath::Max(InRange, 0.f))
	, Cooldown(FMath::Max(InCooldown, 0.f))
	, NextAttackTime(TNumericLimits<double>::Lowest())
{
}

bool FPZMeleeAttack::TryAttack(double Now, double DistanceToTarget)
{
	if (!IsInRange(DistanceToTarget) || Now < NextAttackTime)
	{
		return false;
	}

	NextAttackTime = Now + Cooldown;
	return true;
}
