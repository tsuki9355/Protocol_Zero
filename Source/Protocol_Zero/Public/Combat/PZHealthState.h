// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Outcome of a single damage application. */
enum class EPZDamageResult : uint8
{
	/** No health was removed: zero damage, already dead, or inside the invulnerability window. */
	Ignored,
	/** Health was removed and the owner is still alive. */
	Damaged,
	/** This hit took health to zero. Returned at most once per life. */
	Killed
};

/**
 * Health rules with no engine dependencies, so they can be unit-tested directly.
 * Time is passed in by the caller rather than read from a world.
 */
struct PROTOCOL_ZERO_API FPZHealthState
{
	/** Configures the pool and restores it to full health. */
	FPZHealthState(float InMaxHealth = 100.f, float InInvulnerabilityDuration = 0.f);

	/**
	 * Removes Amount health unless the hit is ignored.
	 * A successful hit starts the invulnerability window at Now.
	 */
	EPZDamageResult ApplyDamage(float Amount, double Now);

	/** Restores full health and clears the invulnerability window. */
	void Reset();

	/** True while a hit at Now would be ignored because of the post-hit grace window. */
	bool IsInvulnerable(double Now) const { return Now < InvulnerableUntil; }

	/** True once health has reached zero. */
	bool IsDead() const { return CurrentHealth <= 0.f; }

	/** Health remaining, between zero and the maximum. */
	float GetCurrentHealth() const { return CurrentHealth; }

	/** Health at the start of each life. */
	float GetMaxHealth() const { return MaxHealth; }

private:
	float MaxHealth;
	float InvulnerabilityDuration;
	float CurrentHealth;
	double InvulnerableUntil;
};
