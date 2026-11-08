// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemies/PZSentryAIController.h"
#include "Enemies/PZDrone.h"
#include "Enemies/PZDroneData.h"
#include "Enemies/PZSentryMath.h"
#include "Combat/PZHealthComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "PZLog.h"

namespace
{
	// When cover blocks the shot, close in until the line clears rather than stopping at range.
	constexpr float BlockedApproachRadius = 100.f;
	// Stop slightly inside preferred range so the hold check passes on arrival.
	constexpr float RangeApproachFraction = 0.9f;
}

APZSentryAIController::APZSentryAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	// Decisions do not need every frame; path following runs on its own tick.
	PrimaryActorTick.TickInterval = 0.1f;
}

bool APZSentryAIController::HasLineOfFire(const APawn* Drone, const AActor* Player) const
{
	FCollisionQueryParams Params(SCENE_QUERY_STAT(PZSentryLineOfFire), false, Drone);
	Params.AddIgnoredActor(Player);
	// Only static geometry (cover and walls) counts; other drones never block the decision.
	return !GetWorld()->LineTraceTestByObjectType(Drone->GetActorLocation(), Player->GetActorLocation(),
		FCollisionObjectQueryParams(ECC_WorldStatic), Params);
}

void APZSentryAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APZDrone* Drone = Cast<APZDrone>(GetPawn());
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Drone || !Player || !Drone->GetDroneData())
	{
		return;
	}

	const UPZHealthComponent* PlayerHealth = Player->FindComponentByClass<UPZHealthComponent>();
	if (PlayerHealth && PlayerHealth->IsDead())
	{
		StopMovement();
		ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}

	const UPZDroneData* Data = Drone->GetDroneData();
	const bool bLineOfFire = HasLineOfFire(Drone, Player);
	const double Distance = FVector::Dist2D(Drone->GetActorLocation(), Player->GetActorLocation());
	const EPZSentryAction Action = PZSentryMath::DecideAction(Distance, Data->PreferredRange, Data->ResumeMargin, bLineOfFire, bHolding);

	if (Action == EPZSentryAction::HoldAndFire)
	{
		if (!bHolding)
		{
			StopMovement();
			bHolding = true;
			UE_LOG(LogProtocolZero, Verbose, TEXT("%s holding at %.0f cm"), *Drone->GetName(), Distance);
		}
		SetFocus(Player);
		Drone->TryRangedAttack(Player);
		return;
	}

	bHolding = false;
	ClearFocus(EAIFocusPriority::Gameplay);
	if (GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		const float Acceptance = bLineOfFire ? Data->PreferredRange * RangeApproachFraction : BlockedApproachRadius;
		MoveToActor(Player, Acceptance, true);
	}
}
