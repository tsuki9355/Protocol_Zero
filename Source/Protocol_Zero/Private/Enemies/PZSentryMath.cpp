// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemies/PZSentryMath.h"

EPZSentryAction PZSentryMath::DecideAction(double DistanceToPlayer, float PreferredRange, float ResumeMargin,
	bool bHasLineOfFire, bool bCurrentlyHolding)
{
	if (!bHasLineOfFire)
	{
		return EPZSentryAction::Approach;
	}

	const double HoldLimit = bCurrentlyHolding ? PreferredRange + FMath::Max(ResumeMargin, 0.f) : PreferredRange;
	return DistanceToPlayer <= HoldLimit ? EPZSentryAction::HoldAndFire : EPZSentryAction::Approach;
}
