// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Combat/PZHealthComponent.h"
#include "Enemies/PZDrone.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Waves/PZSpawnPoint.h"
#include "Waves/PZWaveData.h"
#include "Waves/PZWaveManagerComponent.h"
#include "Waves/PZWaveTally.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZWaveManagerTest
{
	constexpr int32 GroupSize = 2;
	constexpr float HealthMultiplier = 2.f;
	constexpr float BaseHunterHealth = 30.f; // DA_Hunter placeholder
	constexpr float Lethal = 10000.f;
	const FVector FarPoint(1500.0, 0.0, 100.0);

	/** Minimal map-less game world with one spawn point and a wave manager. */
	struct FTestWorld
	{
		UWorld* World = nullptr;
		UPZWaveManagerComponent* Manager = nullptr;

		FTestWorld()
		{
			World = UWorld::CreateWorld(EWorldType::Game, false);
			FWorldContext& Context = GEngine->CreateNewWorldContext(EWorldType::Game);
			Context.SetCurrentWorld(World);
			World->InitializeActorsForPlay(FURL());
			World->BeginPlay();

			World->SpawnActor<APZSpawnPoint>(FarPoint, FRotator::ZeroRotator);

			AActor* Owner = World->SpawnActor<AActor>();
			Manager = NewObject<UPZWaveManagerComponent>(Owner);
			Manager->RandomSeed = 1;
			Manager->RegisterComponent();

			UPZWaveData* Wave = NewObject<UPZWaveData>(Owner);
			Wave->SpawnGroups = { { APZDrone::StaticClass(), GroupSize, 0.f } };
			Wave->HealthMultiplier = HealthMultiplier;
			Manager->Waves = { Wave };
		}

		~FTestWorld()
		{
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(false);
		}

		TArray<APZDrone*> Drones() const
		{
			TArray<APZDrone*> Result;
			for (TActorIterator<APZDrone> It(World); It; ++It)
			{
				Result.Add(*It);
			}
			return Result;
		}
	};
}

#define PZ_WAVE_MANAGER_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTallyCountsDownTest, "Protocol_Zero.Waves.TallyCountsDownToCleared", PZ_WAVE_MANAGER_TEST_FLAGS)
bool FTallyCountsDownTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZWaveTally Tally(2);

	// Act
	Tally.RecordDefeat();
	const int32 AfterOne = Tally.GetRemaining();
	const bool bClearedAfterOne = Tally.IsCleared();
	Tally.RecordDefeat();

	// Assert
	TestEqual(TEXT("one left after one defeat"), AfterOne, 1);
	TestFalse(TEXT("not cleared with one left"), bClearedAfterOne);
	TestTrue(TEXT("cleared after the last defeat"), Tally.IsCleared());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTallyIgnoresExtraDefeatsTest, "Protocol_Zero.Waves.TallyIgnoresExtraDefeats", PZ_WAVE_MANAGER_TEST_FLAGS)
bool FTallyIgnoresExtraDefeatsTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZWaveTally Tally(1);

	// Act
	Tally.RecordDefeat();
	Tally.RecordDefeat();

	// Assert
	TestEqual(TEXT("remaining never goes negative"), Tally.GetRemaining(), 0);
	TestEqual(TEXT("defeats capped at the total"), Tally.GetDefeated(), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveManagerSpawnsGroupTest, "Protocol_Zero.Waves.ManagerSpawnsDueGroupWithMultipliers", PZ_WAVE_MANAGER_TEST_FLAGS)
bool FWaveManagerSpawnsGroupTest::RunTest(const FString& Parameters)
{
	// Arrange
	PZWaveManagerTest::FTestWorld Test;

	// Act
	const bool bStarted = Test.Manager->StartWave(0);
	const TArray<APZDrone*> Drones = Test.Drones();

	// Assert
	TestTrue(TEXT("wave started"), bStarted);
	TestEqual(TEXT("first group spawned at start"), Drones.Num(), PZWaveManagerTest::GroupSize);
	TestEqual(TEXT("enemies remaining"), Test.Manager->GetEnemiesRemaining(), PZWaveManagerTest::GroupSize);
	for (const APZDrone* Drone : Drones)
	{
		TestEqual(TEXT("wave health multiplier applied"), Drone->GetHealth()->GetMaxHealth(),
			PZWaveManagerTest::BaseHunterHealth * PZWaveManagerTest::HealthMultiplier);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveManagerClearsTest, "Protocol_Zero.Waves.ManagerClearsWaveWhenAllDestroyed", PZ_WAVE_MANAGER_TEST_FLAGS)
bool FWaveManagerClearsTest::RunTest(const FString& Parameters)
{
	// Arrange
	PZWaveManagerTest::FTestWorld Test;
	Test.Manager->StartWave(0);
	const TArray<APZDrone*> Drones = Test.Drones();

	// Act
	Drones[0]->GetHealth()->ApplyDamage(PZWaveManagerTest::Lethal, FVector::ZeroVector);
	const bool bActiveWithOneLeft = Test.Manager->IsWaveActive();
	Drones[1]->GetHealth()->ApplyDamage(PZWaveManagerTest::Lethal, FVector::ZeroVector);

	// Assert
	TestTrue(TEXT("wave still active with one drone left"), bActiveWithOneLeft);
	TestEqual(TEXT("no enemies remaining"), Test.Manager->GetEnemiesRemaining(), 0);
	TestFalse(TEXT("wave cleared"), Test.Manager->IsWaveActive());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveManagerBadIndexTest, "Protocol_Zero.Waves.ManagerRejectsMissingWave", PZ_WAVE_MANAGER_TEST_FLAGS)
bool FWaveManagerBadIndexTest::RunTest(const FString& Parameters)
{
	// Arrange
	PZWaveManagerTest::FTestWorld Test;

	// Act
	const bool bStarted = Test.Manager->StartWave(5);

	// Assert
	TestFalse(TEXT("out-of-range wave does not start"), bStarted);
	TestFalse(TEXT("no wave active"), Test.Manager->IsWaveActive());
	return true;
}

#undef PZ_WAVE_MANAGER_TEST_FLAGS

#endif
