// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PZDefeatWidget.generated.h"

/**
 * Minimal defeat prompt: a dimmed full-screen overlay with an outcome line and a Restart button.
 * Builds its own widget tree in C++, so it needs no Widget Blueprint. The full results screen
 * (wave reached, enemies defeated, time, damage) replaces it in Milestone 2.
 *
 * Example:
 *   UPZDefeatWidget* Prompt = CreateWidget<UPZDefeatWidget>(PlayerController);
 *   Prompt->AddToViewport();
 */
UCLASS()
class PROTOCOL_ZERO_API UPZDefeatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UFUNCTION()
	void HandleRestartClicked();
};
