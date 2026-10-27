// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemies/PZDestructionMath.h"

float PZDestructionMath::DestructionScale(float Elapsed, float Duration)
{
	if (Duration <= 0.f)
	{
		return 0.f;
	}

	const float Alpha = FMath::Clamp(Elapsed / Duration, 0.f, 1.f);
	if (Alpha <= SwellFraction)
	{
		return FMath::Lerp(1.f, PeakScale, Alpha / SwellFraction);
	}

	const float CollapseAlpha = (Alpha - SwellFraction) / (1.f - SwellFraction);
	return FMath::Lerp(PeakScale, 0.f, CollapseAlpha);
}
