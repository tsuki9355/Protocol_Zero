// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/PZGameMode.h"
#include "Player/PZPlayerCharacter.h"
#include "Player/PZPlayerController.h"

APZGameMode::APZGameMode()
{
	DefaultPawnClass = APZPlayerCharacter::StaticClass();
	PlayerControllerClass = APZPlayerController::StaticClass();
}
