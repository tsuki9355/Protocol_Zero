// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PZHudWidget.generated.h"

class UTextBlock;
class UPZHealthComponent;

/**
 * Combat HUD. For Milestone 1 it shows the player's health in the top-left corner; wave,
 * enemies remaining and upgrades join it in later milestones. Builds its own widget tree
 * in C++ and never takes mouse input, so clicks still reach the game.
 *
 * Example:
 *   UPZHudWidget* Hud = CreateWidget<UPZHudWidget>(PlayerController);
 *   Hud->AddToViewport();
 *   Hud->TrackHealth(PlayerCharacter->GetHealth());
 */
UCLASS()
class PROTOCOL_ZERO_API UPZHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Shows this health component and updates whenever it takes damage. */
	void TrackHealth(UPZHealthComponent* InHealth);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UFUNCTION()
	void HandleDamaged(float Amount, FVector HitDirection, float CurrentHealth);

	void Refresh();

	UPROPERTY()
	TObjectPtr<UTextBlock> HealthValue;

	TWeakObjectPtr<UPZHealthComponent> Health;
};
