// Copyright Epic Games, Inc. All Rights Reserved.

#include "Waves/PZWaveSchedule.h"

FPZWaveSchedule::FPZWaveSchedule(TArray<float> InGroupDelays)
	: GroupDelays(MoveTemp(InGroupDelays))
{
	for (float& Delay : GroupDelays)
	{
		Delay = FMath::Max(Delay, 0.f);
	}
}

void FPZWaveSchedule::Start(double Now)
{
	bStarted = true;
	StartTime = Now;
	NextGroup = 0;
	NextReleaseTime = GroupDelays.IsEmpty() ? Now : Now + GroupDelays[0];
}

int32 FPZWaveSchedule::ConsumeDueGroups(double Now, TArray<int32>& OutDueGroups)
{
	if (!bStarted)
	{
		return 0;
	}

	int32 Released = 0;
	while (NextGroup < GroupDelays.Num() && NextReleaseTime <= Now)
	{
		OutDueGroups.Add(NextGroup);
		++Released;
		++NextGroup;
		if (NextGroup < GroupDelays.Num())
		{
			NextReleaseTime += GroupDelays[NextGroup];
		}
	}
	return Released;
}

double FPZWaveSchedule::GetReleaseOffset(int32 GroupIndex) const
{
	double Offset = 0.0;
	for (int32 Index = 0; Index <= GroupIndex && Index < GroupDelays.Num(); ++Index)
	{
		Offset += GroupDelays[Index];
	}
	return Offset;
}
