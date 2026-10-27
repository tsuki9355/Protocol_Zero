// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Pure helpers for the drone destruction effect, kept free of actors so they can be unit-tested. */
namespace PZDestructionMath
{
	/** Fraction of the effect spent swelling before the collapse. */
	constexpr float SwellFraction = 0.2f;

	/** Peak body scale reached at the end of the swell. */
	constexpr float PeakScale = 1.3f;

	/**
	 * Body scale multiplier during the destruction effect: a quick swell to PeakScale,
	 * then a collapse to zero by the end.
	 * Example: Body->SetRelativeScale3D(BaseScale * DestructionScale(Elapsed, Duration));
	 * @return 1 at the start, PeakScale at SwellFraction of Duration, 0 at or after Duration.
	 */
	PROTOCOL_ZERO_API float DestructionScale(float Elapsed, float Duration);
}
