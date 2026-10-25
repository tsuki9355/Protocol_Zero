// Copyright Epic Games, Inc. All Rights Reserved.

#include "Combat/PZFireCadence.h"

namespace
{
	constexpr float MinCooldown = 0.01f;
}

FPZFireCadence::FPZFireCadence(float InCooldown)
	: Cooldown(FMath::Max(InCooldown, MinCooldown))
	, NextShotTime(TNumericLimits<double>::Lowest())
	, bTriggerHeld(false)
{
}

void FPZFireCadence::PullTrigger(double Now)
{
	bTriggerHeld = true;
	// Never schedule earlier than now, so a long pause cannot bank a burst of shots.
	NextShotTime = FMath::Max(NextShotTime, Now);
}

void FPZFireCadence::ReleaseTrigger()
{
	bTriggerHeld = false;
}

int32 FPZFireCadence::ConsumeDueShots(double Now)
{
	if (!bTriggerHeld)
	{
		return 0;
	}

	int32 Shots = 0;
	while (NextShotTime <= Now)
	{
		++Shots;
		NextShotTime += Cooldown;
	}
	return Shots;
}

void FPZFireCadence::SetCooldown(float InCooldown)
{
	Cooldown = FMath::Max(InCooldown, MinCooldown);
}
