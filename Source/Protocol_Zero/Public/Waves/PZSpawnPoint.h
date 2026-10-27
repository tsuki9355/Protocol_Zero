// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PZSpawnPoint.generated.h"

class UArrowComponent;

/**
 * Marks an arena-edge location where drones enter. Placed by hand in the arena map;
 * the wave system picks from these. The arrow shows the facing new drones start with.
 * Invisible in game.
 */
UCLASS()
class PROTOCOL_ZERO_API APZSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	APZSpawnPoint();

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UArrowComponent> Arrow;
};
