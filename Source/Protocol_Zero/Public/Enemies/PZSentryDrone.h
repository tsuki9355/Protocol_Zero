// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/PZDrone.h"
#include "PZSentryDrone.generated.h"

/**
 * Slower ranged drone. Paths to its preferred range, holds position and fires enemy bolts at
 * the player on a cooldown. Tuning comes from DA_Sentry; behaviour from APZSentryAIController.
 * A greybox cone body tells it apart from the Hunter's sphere.
 */
UCLASS()
class PROTOCOL_ZERO_API APZSentryDrone : public APZDrone
{
	GENERATED_BODY()

public:
	APZSentryDrone();
};
