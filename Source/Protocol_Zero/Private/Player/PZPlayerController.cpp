// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/PZPlayerController.h"
#include "Player/PZPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"

APZPlayerController::APZPlayerController()
{
	bShowMouseCursor = true;
}

void APZPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void APZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!DefaultMappingContext || !MoveAction)
	{
		BuildDefaultInput();
	}

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APZPlayerController::HandleMove);
	}
}

void APZPlayerController::BuildDefaultInput()
{
	MoveAction = NewObject<UInputAction>(this, TEXT("IA_Move"));
	MoveAction->ValueType = EInputActionValueType::Axis2D;

	DefaultMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_Default"));

	// The raw key value lands on X. Swizzle moves it to Y for forward/back; Negate flips it.
	auto MapMoveKey = [this](const FKey& Key, bool bForwardAxis, bool bNegate)
	{
		FEnhancedActionKeyMapping& Mapping = DefaultMappingContext->MapKey(MoveAction, Key);
		if (bForwardAxis)
		{
			Mapping.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>(DefaultMappingContext));
		}
		if (bNegate)
		{
			Mapping.Modifiers.Add(NewObject<UInputModifierNegate>(DefaultMappingContext));
		}
	};

	MapMoveKey(EKeys::W, true, false);
	MapMoveKey(EKeys::S, true, true);
	MapMoveKey(EKeys::D, false, false);
	MapMoveKey(EKeys::A, false, true);
}

void APZPlayerController::HandleMove(const FInputActionValue& Value)
{
	if (APZPlayerCharacter* PlayerCharacter = Cast<APZPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Move(Value.Get<FVector2D>());
	}
}
