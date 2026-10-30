// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PZWaveData.generated.h"

class APZDrone;

/** One timed group of identical drones within a wave. */
USTRUCT(BlueprintType)
struct PROTOCOL_ZERO_API FPZSpawnGroup
{
	GENERATED_BODY()

	/** Drone type to spawn: the Hunter (APZDrone) or, from M2-S04, the Sentry. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<APZDrone> DroneClass;

	/** Drones in this group, spread across spawn points. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (ClampMin = "1"))
	int32 Count = 1;

	/** Seconds after the previous group (or the wave start, for the first group) before this group arrives. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (ClampMin = "0"))
	float DelayAfterPrevious = 0.f;
};

/**
 * One wave of a run: ordered spawn groups plus the difficulty multipliers applied to every
 * drone in the wave. A run uses DA_Wave_01..05 in /Game/Data/Waves.
 * Values are placeholders until the tuning pass; the design leaves exact numbers open.
 */
UCLASS(BlueprintType)
class PROTOCOL_ZERO_API UPZWaveData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Groups in arrival order. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FPZSpawnGroup> SpawnGroups;

	/** Multiplier on every drone's base health this wave. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty", meta = (ClampMin = "0.1"))
	float HealthMultiplier = 1.f;

	/** Multiplier on every drone's base damage this wave. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty", meta = (ClampMin = "0.1"))
	float DamageMultiplier = 1.f;

	/** Total drones across all groups. */
	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetTotalDroneCount() const;

	/** Each group's delay, in order, for building an FPZWaveSchedule. */
	TArray<float> GetGroupDelays() const;
};
