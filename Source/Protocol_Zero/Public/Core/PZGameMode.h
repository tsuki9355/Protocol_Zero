// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PZGameMode.generated.h"

class UPZWaveManagerComponent;

/**
 * Project game mode for Protocol Zero, assigned as the global default in DefaultEngine.ini.
 *
 * Owns run flow (waves, countdown, upgrade selection, victory and defeat) as later stories add it.
 * Until the run state machine lands (M2-S06), it simply starts Wave 1 when play begins.
 */
UCLASS()
class PROTOCOL_ZERO_API APZGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APZGameMode();

	/** Spawns and tracks each wave's drones. */
	UPZWaveManagerComponent* GetWaveManager() const { return WaveManager; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPZWaveManagerComponent> WaveManager;
};
