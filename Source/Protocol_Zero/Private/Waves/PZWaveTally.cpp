// Copyright Epic Games, Inc. All Rights Reserved.

#include "Waves/PZWaveTally.h"

FPZWaveTally::FPZWaveTally(int32 InTotal)
	: Total(FMath::Max(InTotal, 0))
{
}

void FPZWaveTally::RecordDefeat()
{
	Defeated = FMath::Min(Defeated + 1, Total);
}
