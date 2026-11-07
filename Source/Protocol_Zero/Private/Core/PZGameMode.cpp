// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/PZGameMode.h"
#include "Player/PZPlayerCharacter.h"
#include "Player/PZPlayerController.h"
#include "Waves/PZWaveData.h"
#include "Waves/PZWaveManagerComponent.h"
#include "UObject/ConstructorHelpers.h"

APZGameMode::APZGameMode()
{
	DefaultPawnClass = APZPlayerCharacter::StaticClass();
	PlayerControllerClass = APZPlayerController::StaticClass();

	WaveManager = CreateDefaultSubobject<UPZWaveManagerComponent>(TEXT("WaveManager"));

	static ConstructorHelpers::FObjectFinder<UPZWaveData> Wave01(TEXT("/Game/Data/Waves/DA_Wave_01.DA_Wave_01"));
	if (Wave01.Succeeded())
	{
		WaveManager->Waves.Add(Wave01.Object);
	}
}

void APZGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Temporary until the run state machine (M2-S06) drives waves.
	WaveManager->StartWave(0);
}
