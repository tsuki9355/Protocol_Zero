// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Counts one wave's drones, with no engine dependencies so it can be unit-tested.
 * "Remaining" includes drones that have not spawned yet, so it is the number the player
 * still has to destroy. The wave is cleared once every drone in it has been destroyed.
 *
 * Example:
 *   FPZWaveTally Tally(Wave->GetTotalDroneCount());
 *   Tally.RecordDefeat();
 *   if (Tally.IsCleared()) { ... }
 */
struct PROTOCOL_ZERO_API FPZWaveTally
{
	/** @param InTotal Drones in the whole wave, spawned or not. */
	explicit FPZWaveTally(int32 InTotal = 0);

	/** Records one drone destroyed. Extra calls past the total are ignored. */
	void RecordDefeat();

	/** Drones still to destroy, including those not spawned yet. */
	int32 GetRemaining() const { return Total - Defeated; }

	/** Drones destroyed so far this wave. */
	int32 GetDefeated() const { return Defeated; }

	/** True once every drone in the wave has been destroyed. */
	bool IsCleared() const { return Defeated >= Total; }

private:
	int32 Total;
	int32 Defeated = 0;
};
