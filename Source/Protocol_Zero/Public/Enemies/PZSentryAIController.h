// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PZSentryAIController.generated.h"

/**
 * Sentry behaviour: paths toward the player around cover until it is within its preferred
 * range with a clear line of fire, then stands still and fires on cooldown. It never
 * retreats; it repositions only when the player moves out of range or behind cover.
 */
UCLASS()
class PROTOCOL_ZERO_API APZSentryAIController : public AAIController
{
	GENERATED_BODY()

public:
	APZSentryAIController();

	virtual void Tick(float DeltaSeconds) override;

private:
	/** True if nothing static blocks a straight shot from the drone to the player. */
	bool HasLineOfFire(const APawn* Drone, const AActor* Player) const;

	bool bHolding = false;
};
