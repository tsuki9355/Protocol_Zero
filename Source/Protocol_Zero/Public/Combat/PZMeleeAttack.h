// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Short-range attack on a cooldown, with no engine dependencies so it can be unit-tested.
 * Damage is only ever dealt through a successful TryAttack, never by contact.
 *
 * Example:
 *   FPZMeleeAttack Melee(150.f, 1.f);
 *   if (Melee.TryAttack(Now, DistanceToTarget)) { Target->ApplyDamage(...); }
 */
struct PROTOCOL_ZERO_API FPZMeleeAttack
{
	/**
	 * @param InRange Maximum centre-to-centre distance for a hit, in cm.
	 * @param InCooldown Seconds between attacks.
	 */
	FPZMeleeAttack(float InRange = 150.f, float InCooldown = 1.f);

	/** Attacks if the target is in range and the cooldown has finished. Starts the cooldown on success. */
	bool TryAttack(double Now, double DistanceToTarget);

	/** True if a target at this distance can be hit. */
	bool IsInRange(double DistanceToTarget) const { return DistanceToTarget <= Range; }

	/** Maximum centre-to-centre distance for a hit. */
	float GetRange() const { return Range; }

private:
	float Range;
	float Cooldown;
	double NextAttackTime;
};
