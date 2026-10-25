// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PZPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * Routes Enhanced Input to the player character and keeps the mouse cursor visible for aiming.
 * If no input assets are assigned, it builds the default WASD + left-mouse mapping at runtime.
 */
UCLASS()
class PROTOCOL_ZERO_API APZPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APZPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	/** Mapping context added on BeginPlay. Optional; a WASD default is built when empty. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Axis2D move action. Optional; built with the default mapping when empty. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Boolean fire action, held for automatic fire. Optional; built with the default mapping when empty. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

private:
	void BuildDefaultInput();
	void HandleMove(const FInputActionValue& Value);
	void HandleFireStarted();
	void HandleFireStopped();
};
