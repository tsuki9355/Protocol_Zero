// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemies/PZHunterAIController.h"
#include "Enemies/PZDrone.h"
#include "Combat/PZHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "PZLog.h"

APZHunterAIController::APZHunterAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	// Decisions do not need every frame; path following runs on its own tick.
	PrimaryActorTick.TickInterval = 0.1f;
}

void APZHunterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APZDrone* Drone = Cast<APZDrone>(GetPawn());
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Drone || !Player)
	{
		return;
	}

	const UPZHealthComponent* PlayerHealth = Player->FindComponentByClass<UPZHealthComponent>();
	if (PlayerHealth && PlayerHealth->IsDead())
	{
		StopMovement();
		return;
	}

	if (Drone->IsInMeleeRange(Player))
	{
		StopMovement();
		SetFocus(Player);
		Drone->TryMeleeAttack(Player);
		return;
	}

	ClearFocus(EAIFocusPriority::Gameplay);
	if (GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		const EPathFollowingRequestResult::Type Result = MoveToActor(Player, ApproachAcceptanceRadius, true);
		if (Result == EPathFollowingRequestResult::Failed)
		{
			UE_LOG(LogProtocolZero, Verbose, TEXT("%s could not path to the player"), *Drone->GetName());
		}
	}
}
