// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/PZPlayerController.h"
#include "Player/PZAimMath.h"
#include "Player/PZPlayerCharacter.h"
#include "Combat/PZHealthComponent.h"
#include "Combat/PZWeaponComponent.h"
#include "UI/PZDefeatWidget.h"
#include "UI/PZHudWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PZLog.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"

APZPlayerController::APZPlayerController()
{
	bShowMouseCursor = true;
	DefeatWidgetClass = UPZDefeatWidget::StaticClass();
	HudWidgetClass = UPZHudWidget::StaticClass();
}

void APZPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	bPlayerDead = false;
	if (APZPlayerCharacter* PlayerCharacter = Cast<APZPlayerCharacter>(InPawn))
	{
		PlayerCharacter->GetHealth()->OnDeath.AddUniqueDynamic(this, &APZPlayerController::HandlePawnDeath);

		if (!Hud && HudWidgetClass && IsLocalController())
		{
			Hud = CreateWidget<UPZHudWidget>(this, HudWidgetClass);
			Hud->AddToViewport();
		}
		if (Hud)
		{
			Hud->TrackHealth(PlayerCharacter->GetHealth());
		}
	}
}

void APZPlayerController::HandlePawnDeath()
{
	bPlayerDead = true;
	if (APZPlayerCharacter* PlayerCharacter = Cast<APZPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->GetWeapon()->StopFire();
	}
}

void APZPlayerController::ShowDefeat()
{
	if (DefeatWidgetClass)
	{
		if (UPZDefeatWidget* Prompt = CreateWidget<UPZDefeatWidget>(this, DefeatWidgetClass))
		{
			Prompt->SetIsFocusable(true);
			Prompt->AddToViewport();
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(Prompt->TakeWidget());
			SetInputMode(InputMode);
		}
	}
	UE_LOG(LogProtocolZero, Log, TEXT("Defeat prompt shown"));
}

void APZPlayerController::RestartRun()
{
	UE_LOG(LogProtocolZero, Log, TEXT("Restarting run"));
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
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

	if (!DefaultMappingContext || !MoveAction || !FireAction)
	{
		BuildDefaultInput();
	}

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APZPlayerController::HandleMove);
		Input->BindAction(FireAction, ETriggerEvent::Started, this, &APZPlayerController::HandleFireStarted);
		Input->BindAction(FireAction, ETriggerEvent::Completed, this, &APZPlayerController::HandleFireStopped);
	}
}

void APZPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	APZPlayerCharacter* PlayerCharacter = Cast<APZPlayerCharacter>(GetPawn());
	if (!PlayerCharacter || bPlayerDead)
	{
		return;
	}

	// Aim on the horizontal plane through the character, so empty floor and open space both work.
	FVector RayOrigin;
	FVector RayDirection;
	FVector AimPoint;
	if (DeprojectMousePositionToWorld(RayOrigin, RayDirection)
		&& PZAimMath::ProjectCursorToPlane(RayOrigin, RayDirection, PlayerCharacter->GetActorLocation().Z, AimPoint))
	{
		PlayerCharacter->AimAt(AimPoint);
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

	FireAction = NewObject<UInputAction>(this, TEXT("IA_Fire"));
	FireAction->ValueType = EInputActionValueType::Boolean;
	DefaultMappingContext->MapKey(FireAction, EKeys::LeftMouseButton);
}

void APZPlayerController::HandleFireStarted()
{
	if (bPlayerDead)
	{
		return;
	}

	if (APZPlayerCharacter* PlayerCharacter = Cast<APZPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->GetWeapon()->StartFire();
	}
}

void APZPlayerController::HandleFireStopped()
{
	if (APZPlayerCharacter* PlayerCharacter = Cast<APZPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->GetWeapon()->StopFire();
	}
}

void APZPlayerController::HandleMove(const FInputActionValue& Value)
{
	if (bPlayerDead)
	{
		return;
	}

	if (APZPlayerCharacter* PlayerCharacter = Cast<APZPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Move(Value.Get<FVector2D>());
	}
}
