// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/PZProjectile.h"
#include "PZDroneData.generated.h"

/**
 * Tuning for one drone type: DA_Hunter and DA_Sentry in /Game/Data/Enemies.
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

	/** Centre-to-centre distance at which the attack can be used, in cm: melee reach, or maximum firing distance for ranged drones. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0"))
	float AttackRange = 150.f;

	/** Damage per melee hit or shot, before the wave multiplier. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0"))
	float AttackDamage = 10.f;

	/** Seconds between melee hits or shots. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0"))
	float AttackCooldown = 1.f;

	/** Ranged drones only: distance they close to before holding position and firing, in cm. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged", meta = (ClampMin = "0"))
	float PreferredRange = 0.f;

	/** Ranged drones only: how far past PreferredRange the player can move before the drone repositions, in cm. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged", meta = (ClampMin = "0"))
	float ResumeMargin = 150.f;

	/** Ranged drones only: projectile fired by the attack. Empty for melee drones. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
	TSubclassOf<APZProjectile> ProjectileClass;

	/** Ranged drones only: distance in front of the drone where shots spawn, in cm. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged", meta = (ClampMin = "0"))
	float MuzzleOffset = 60.f;

	/** Length of the destruction effect before the drone despawns, in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death", meta = (ClampMin = "0"))
	float DestructionDuration = 0.4f;

	/** Spin speed of the body during the destruction effect, in degrees per second. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	float DestructionSpinRate = 900.f;
};
