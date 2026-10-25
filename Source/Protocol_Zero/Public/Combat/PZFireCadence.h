// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Automatic-fire timing with no engine dependencies, so it can be unit-tested.
 * Counts every shot that came due since the last call, so the fire rate does not
 * depend on the frame rate. Time is passed in by the caller.
 *
 * Example:
 *   FPZFireCadence Cadence(0.1f);
 *   Cadence.PullTrigger(Now);
 *   const int32 Shots = Cadence.ConsumeDueShots(Now); // call every frame while held
 */
struct PROTOCOL_ZERO_API FPZFireCadence
{
	/** @param InCooldown Seconds between shots. */
	explicit FPZFireCadence(float InCooldown = 0.1f);

	/** Trigger pressed. Fires at once unless the previous shot's cooldown is still running. */
	void PullTrigger(double Now);

	/** Trigger released. Shots stop; the cooldown keeps running. */
	void ReleaseTrigger();

	/** Returns how many shots came due by Now while the trigger is held, and advances the schedule. */
	int32 ConsumeDueShots(double Now);

	/** Changes the cooldown. Takes effect from the next scheduled shot. */
	void SetCooldown(float InCooldown);

	/** True while the trigger is held. */
	bool IsTriggerHeld() const { return bTriggerHeld; }

	/** Seconds between shots. */
	float GetCooldown() const { return Cooldown; }

private:
	float Cooldown;
	double NextShotTime;
	bool bTriggerHeld;
};
