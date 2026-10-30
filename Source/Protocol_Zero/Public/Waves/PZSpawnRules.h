// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Pure spawn-point selection, kept free of actors so it can be unit-tested. */
namespace PZSpawnRules
{
	/**
	 * Chooses a spawn point for each of Count drones. Only points at least MinDistance from the
	 * player (on the ground plane) are eligible, and drones are spread across eligible points in
	 * a shuffled order before any point is reused. If no point qualifies, every drone uses the
	 * point farthest from the player, so a group always spawns.
	 *
	 * Example:
	 *   FRandomStream Random(Seed);
	 *   const TArray<int32> Picks = ChooseSpawnPoints(PointLocations, PlayerLocation, 800.0, 3, Random);
	 *
	 * @return Count indices into SpawnPoints, or an empty array when there are no spawn points.
	 */
	PROTOCOL_ZERO_API TArray<int32> ChooseSpawnPoints(const TArray<FVector>& SpawnPoints, const FVector& PlayerLocation,
		double MinDistance, int32 Count, FRandomStream& Random);
}
