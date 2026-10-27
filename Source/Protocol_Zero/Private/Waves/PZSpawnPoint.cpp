// Copyright Epic Games, Inc. All Rights Reserved.

#include "Waves/PZSpawnPoint.h"
#include "Components/ArrowComponent.h"

APZSpawnPoint::APZSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->ArrowColor = FColor::Orange;
	Arrow->ArrowSize = 2.f;
	Arrow->bIsScreenSizeScaled = true;

	SetActorHiddenInGame(true);
}
