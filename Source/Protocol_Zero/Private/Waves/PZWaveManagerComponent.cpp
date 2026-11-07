// Copyright Epic Games, Inc. All Rights Reserved.

#include "Waves/PZWaveManagerComponent.h"
#include "Waves/PZSpawnPoint.h"
#include "Waves/PZSpawnRules.h"
#include "Waves/PZWaveData.h"
#include "Combat/PZHealthComponent.h"
#include "Enemies/PZDrone.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "PZLog.h"

UPZWaveManagerComponent::UPZWaveManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPZWaveManagerComponent::GatherSpawnPoints()
{
	SpawnPoints.Reset();
	for (TActorIterator<APZSpawnPoint> It(GetWorld()); It; ++It)
	{
		SpawnPoints.Add(*It);
	}
}

bool UPZWaveManagerComponent::StartWave(int32 WaveIndex)
{
	if (!Waves.IsValidIndex(WaveIndex) || !Waves[WaveIndex])
	{
		UE_LOG(LogProtocolZero, Warning, TEXT("No wave data at index %d"), WaveIndex);
		return false;
	}

	if (SpawnPoints.IsEmpty())
	{
		GatherSpawnPoints();
		Random.Initialize(RandomSeed != 0 ? RandomSeed : FMath::Rand());
	}

	CurrentWaveIndex = WaveIndex;
	CurrentWave = Waves[WaveIndex];
	Tally = FPZWaveTally(CurrentWave->GetTotalDroneCount());
	Schedule = FPZWaveSchedule(CurrentWave->GetGroupDelays());
	Schedule.Start(GetWorld()->GetTimeSeconds());
	bWaveActive = true;
	SetComponentTickEnabled(true);

	UE_LOG(LogProtocolZero, Log, TEXT("Wave %d started: %d drones in %d groups"),
		GetCurrentWaveNumber(), Tally.GetRemaining(), Schedule.NumGroups());
	OnWaveStarted.Broadcast(GetCurrentWaveNumber());

	Update();
	return true;
}

void UPZWaveManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Update();
}

void UPZWaveManagerComponent::Update()
{
	if (!bWaveActive)
	{
		return;
	}

	DueGroups.Reset();
	Schedule.ConsumeDueGroups(GetWorld()->GetTimeSeconds(), DueGroups);
	for (const int32 GroupIndex : DueGroups)
	{
		SpawnGroup(GroupIndex);
	}

	if (Schedule.IsFinished())
	{
		// Nothing left to release; drone deaths finish the wave from here.
		SetComponentTickEnabled(false);
	}
}

void UPZWaveManagerComponent::SpawnGroup(int32 GroupIndex)
{
	const FPZSpawnGroup& Group = CurrentWave->SpawnGroups[GroupIndex];
	UWorld* World = GetWorld();
	if (!Group.DroneClass || SpawnPoints.IsEmpty())
	{
		UE_LOG(LogProtocolZero, Warning, TEXT("Wave %d group %d skipped: no drone class or no spawn points"), GetCurrentWaveNumber(), GroupIndex);
		for (int32 Index = 0; Index < Group.Count; ++Index)
		{
			HandleDroneDeath();
		}
		return;
	}

	TArray<FVector> PointLocations;
	PointLocations.Reserve(SpawnPoints.Num());
	for (const APZSpawnPoint* Point : SpawnPoints)
	{
		PointLocations.Add(Point->GetActorLocation());
	}

	const APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	const FVector PlayerLocation = Player ? Player->GetActorLocation() : FVector::ZeroVector;
	const TArray<int32> Picks = PZSpawnRules::ChooseSpawnPoints(PointLocations, PlayerLocation, MinSpawnDistance, Group.Count, Random);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const int32 PointIndex : Picks)
	{
		const APZSpawnPoint* Point = SpawnPoints[PointIndex];
		APZDrone* Drone = World->SpawnActor<APZDrone>(Group.DroneClass, Point->GetActorLocation(), Point->GetActorRotation(), Params);
		if (!Drone)
		{
			HandleDroneDeath();
			continue;
		}

		Drone->ApplyWaveMultipliers(CurrentWave->HealthMultiplier, CurrentWave->DamageMultiplier);
		Drone->GetHealth()->OnDeath.AddDynamic(this, &UPZWaveManagerComponent::HandleDroneDeath);
	}

	UE_LOG(LogProtocolZero, Log, TEXT("Wave %d group %d: spawned %d %s"),
		GetCurrentWaveNumber(), GroupIndex + 1, Picks.Num(), *Group.DroneClass->GetName());
}

void UPZWaveManagerComponent::HandleDroneDeath()
{
	if (!bWaveActive)
	{
		return;
	}

	Tally.RecordDefeat();
	OnEnemyDefeated.Broadcast(GetCurrentWaveNumber(), Tally.GetRemaining());

	if (Tally.IsCleared())
	{
		bWaveActive = false;
		SetComponentTickEnabled(false);
		UE_LOG(LogProtocolZero, Log, TEXT("Wave %d cleared"), GetCurrentWaveNumber());
		OnWaveCleared.Broadcast(GetCurrentWaveNumber());
	}
}
