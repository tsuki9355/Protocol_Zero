// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PZHunterAIController.generated.h"

/**
 * Hunter behaviour: follows the navmesh to the player around cover and, once in range,
 * stops and attacks on the drone's melee cooldown.
 */
UCLASS()
class PROTOCOL_ZERO_API APZHunterAIController : public AAIController
{
	GENERATED_BODY()

public:
	APZHunterAIController();

	virtual void Tick(float DeltaSeconds) override;

protected:
	/** Extra distance inside melee range the Hunter closes to before stopping, in cm. */
	UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "0"))
	float ApproachAcceptanceRadius = 50.f;
};
