// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemies/PZSentryDrone.h"
#include "Enemies/PZDroneData.h"
#include "Enemies/PZSentryAIController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

APZSentryDrone::APZSentryDrone()
{
	AIControllerClass = APZSentryAIController::StaticClass();

	// Face the controller's focus (the player) while holding, and the path while moving.
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
	if (ConeMesh.Succeeded())
	{
		Body->SetStaticMesh(ConeMesh.Object);
		Body->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.9f));
	}

	static ConstructorHelpers::FObjectFinder<UPZDroneData> SentryData(TEXT("/Game/Data/Enemies/DA_Sentry.DA_Sentry"));
	if (SentryData.Succeeded())
	{
		DroneData = SentryData.Object;
	}
}
