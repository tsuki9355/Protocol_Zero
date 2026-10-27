// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PZDroneData.generated.h"

/**
 * Tuning for one drone type. The Hunter is DA_Hunter in /Game/Data/Enemies.
 * Values are placeholders until the tuning pass; the design leaves exact numbers open.
 */
UCLASS(BlueprintType)
class PROTOCOL_ZERO_API UPZDroneData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Health at spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone", meta = (ClampMin = "1"))
	float MaxHealth = 30.f;

	/** Ground speed in cm/s. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone", meta = (ClampMin = "0"))
	float MoveSpeed = 550.f;

	/** Centre-to-centre distance at which the melee attack connects, in cm. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0"))
	float AttackRange = 150.f;

	/** Damage per melee hit. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0"))
	float AttackDamage = 10.f;

	/** Seconds between melee hits. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0"))
	float AttackCooldown = 1.f;
};
