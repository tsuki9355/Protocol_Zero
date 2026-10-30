// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Pure HUD text helpers, kept free of widgets so they can be unit-tested. */
namespace PZHudFormat
{
	/**
	 * Health readout as whole numbers, "current / max". Current rounds up, so a sliver of
	 * health never reads as 0 while the player is alive; it is clamped to [0, max].
	 * Example: HealthText(89.2f, 100.f) -> "90 / 100".
	 */
	PROTOCOL_ZERO_API FText HealthText(float Current, float Max);
}
