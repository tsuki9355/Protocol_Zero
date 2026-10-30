// Copyright Epic Games, Inc. All Rights Reserved.

#include "Waves/PZWaveData.h"
#include "Enemies/PZDrone.h"

int32 UPZWaveData::GetTotalDroneCount() const
{
	int32 Total = 0;
	for (const FPZSpawnGroup& Group : SpawnGroups)
	{
		Total += FMath::Max(Group.Count, 0);
	}
	return Total;
}

TArray<float> UPZWaveData::GetGroupDelays() const
{
	TArray<float> Delays;
	Delays.Reserve(SpawnGroups.Num());
	for (const FPZSpawnGroup& Group : SpawnGroups)
	{
		Delays.Add(Group.DelayAfterPrevious);
	}
	return Delays;
}
