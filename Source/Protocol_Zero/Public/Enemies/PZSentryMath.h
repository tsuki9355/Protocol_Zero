// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** What a Sentry does this decision tick. */
enum class EPZSentryAction : uint8
{
	/** Path toward the player: too far away, or cover blocks the shot. */
	Approach,
	/** Stand still and fire on cooldown. */
	HoldAndFire
};

/** Pure Sentry positioning rules, kept free of actors so they can be unit-tested. */
namespace PZSentryMath
{
	/**
	 * Decides whether a Sentry approaches or holds. It holds once it is within PreferredRange with
	 * a clear line of fire, and stays put (it never retreats) until the player moves more than
	 * ResumeMargin beyond PreferredRange or cover blocks the shot. The margin stops it from
	 * twitching between moving and firing at the edge of its range.
	 *
	 * Example:
	 *   const EPZSentryAction Action = DecideAction(Distance, 1000.f, 150.f, bHasLineOfFire, bHolding);
	 */
	PROTOCOL_ZERO_API EPZSentryAction DecideAction(double DistanceToPlayer, float PreferredRange, float ResumeMargin,
		bool bHasLineOfFire, bool bCurrentlyHolding);
}
