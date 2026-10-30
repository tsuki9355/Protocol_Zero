// Copyright Epic Games, Inc. All Rights Reserved.

#include "Waves/PZSpawnRules.h"

TArray<int32> PZSpawnRules::ChooseSpawnPoints(const TArray<FVector>& SpawnPoints, const FVector& PlayerLocation,
	double MinDistance, int32 Count, FRandomStream& Random)
{
	TArray<int32> Picks;
	if (SpawnPoints.IsEmpty() || Count <= 0)
	{
		return Picks;
	}

	TArray<int32> Eligible;
	int32 Farthest = 0;
	double FarthestDistance = -1.0;
	for (int32 Index = 0; Index < SpawnPoints.Num(); ++Index)
	{
		const double Distance = FVector::Dist2D(SpawnPoints[Index], PlayerLocation);
		if (Distance >= MinDistance)
		{
			Eligible.Add(Index);
		}
		if (Distance > FarthestDistance)
		{
			FarthestDistance = Distance;
			Farthest = Index;
		}
	}

	if (Eligible.IsEmpty())
	{
		Picks.Init(Farthest, Count);
		return Picks;
	}

	// Fisher-Yates shuffle, then walk the shuffled list so every point is used before any repeats.
	for (int32 Index = Eligible.Num() - 1; Index > 0; --Index)
	{
		Eligible.Swap(Index, Random.RandRange(0, Index));
	}

	Picks.Reserve(Count);
	for (int32 Drone = 0; Drone < Count; ++Drone)
	{
		Picks.Add(Eligible[Drone % Eligible.Num()]);
	}
	return Picks;
}
