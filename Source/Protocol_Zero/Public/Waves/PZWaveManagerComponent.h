// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Waves/PZWaveSchedule.h"
#include "Waves/PZWaveTally.h"
#include "PZWaveManagerComponent.generated.h"

class APZSpawnPoint;
class UPZWaveData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPZOnWaveStarted, int32, WaveNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPZOnEnemyDefeated, int32, WaveNumber, int32, EnemiesRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPZOnWaveCleared, int32, WaveNumber);

/**
 * Runs one wave at a time: releases spawn groups on the wave's schedule, places each drone at
 * an edge spawn point outside the minimum distance from the player, applies the wave's health
 * and damage multipliers, and reports enemies remaining and wave cleared. Lives on the GameMode;
 * the run state machine decides when to start each wave.
 *
 * Example:
 *   WaveManager->OnWaveCleared.AddDynamic(this, &AMyMode::HandleWaveCleared);
 *   WaveManager->StartWave(0);
 */
UCLASS(ClassGroup = (ProtocolZero), meta = (BlueprintSpawnableComponent))
class PROTOCOL_ZERO_API UPZWaveManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPZWaveManagerComponent();

	/**
	 * Starts the wave at WaveIndex in Waves (0-based). Groups due at once spawn immediately.
	 * @return False if the index is out of range or the wave asset is missing.
	 */
	bool StartWave(int32 WaveIndex);

	/** Releases every group that is due. Runs on tick while a wave is active. */
	void Update();

	/** Number of waves in the run. */
	int32 GetNumWaves() const { return Waves.Num(); }

	/** 1-based number of the current or last wave, or 0 before the first. */
	int32 GetCurrentWaveNumber() const { return CurrentWaveIndex + 1; }

	/** Drones still to destroy this wave, including those not spawned yet. */
	int32 GetEnemiesRemaining() const { return Tally.GetRemaining(); }

	/** True while a wave is running and not yet cleared. */
	bool IsWaveActive() const { return bWaveActive; }

	/** Fires when a wave starts. */
	UPROPERTY(BlueprintAssignable, Category = "Waves")
	FPZOnWaveStarted OnWaveStarted;

	/** Fires each time a drone of the current wave is destroyed. */
	UPROPERTY(BlueprintAssignable, Category = "Waves")
	FPZOnEnemyDefeated OnEnemyDefeated;

	/** Fires once, when the last drone of the wave is destroyed. */
	UPROPERTY(BlueprintAssignable, Category = "Waves")
	FPZOnWaveCleared OnWaveCleared;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** The run's waves in order. Wave 1 is index 0. */
	UPROPERTY(EditAnywhere, Category = "Tuning")
	TArray<TObjectPtr<UPZWaveData>> Waves;

	/** Drones never spawn closer than this to the player, in cm. */
	UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0"))
	float MinSpawnDistance = 800.f;

	/** Seed for spawn-point choice. Zero picks a new seed each run. */
	UPROPERTY(EditAnywhere, Category = "Tuning")
	int32 RandomSeed = 0;

private:
	void SpawnGroup(int32 GroupIndex);
	void GatherSpawnPoints();

	UFUNCTION()
	void HandleDroneDeath();

	UPROPERTY()
	TArray<TObjectPtr<APZSpawnPoint>> SpawnPoints;

	UPROPERTY()
	TObjectPtr<UPZWaveData> CurrentWave;

	FPZWaveSchedule Schedule;
	FPZWaveTally Tally;
	FRandomStream Random;
	int32 CurrentWaveIndex = -1;
	bool bWaveActive = false;
	TArray<int32> DueGroups;
};
