// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PZGameMode.generated.h"

/**
 * Project game mode for Protocol Zero, assigned as the global default in DefaultEngine.ini.
 *
 * Owns run flow (intro, waves, upgrade selection, victory and defeat) as later stories add it.
 * Blueprint subclasses set pawn and HUD classes once those exist.
 */
UCLASS()
class PROTOCOL_ZERO_API APZGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APZGameMode();
};
