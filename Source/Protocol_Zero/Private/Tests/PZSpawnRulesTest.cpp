// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Waves/PZSpawnRules.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZSpawnTest
{
	constexpr int32 Seed = 1234;
	constexpr double MinDistance = 800.0;
	// Four points on a line; the player stands at the origin.
	const TArray<FVector> Points = {
		FVector(300.0, 0.0, 0.0),   // 0: too close
		FVector(900.0, 0.0, 0.0),   // 1: eligible
		FVector(-1500.0, 0.0, 0.0), // 2: eligible, farthest
		FVector(0.0, 500.0, 0.0),   // 3: too close
	};
	const FVector Player = FVector::ZeroVector;
}

#define PZ_SPAWN_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnNeverTooCloseTest, "Protocol_Zero.Spawn.NeverPicksPointsTooClose", PZ_SPAWN_TEST_FLAGS)
bool FSpawnNeverTooCloseTest::RunTest(const FString& Parameters)
{
	// Arrange
	FRandomStream Random(PZSpawnTest::Seed);
	constexpr int32 ManyDrones = 50;

	// Act
	const TArray<int32> Picks = PZSpawnRules::ChooseSpawnPoints(PZSpawnTest::Points, PZSpawnTest::Player, PZSpawnTest::MinDistance, ManyDrones, Random);

	// Assert
	TestEqual(TEXT("one pick per drone"), Picks.Num(), ManyDrones);
	TestFalse(TEXT("close point 0 never used"), Picks.Contains(0));
	TestFalse(TEXT("close point 3 never used"), Picks.Contains(3));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnFallbackFarthestTest, "Protocol_Zero.Spawn.FallsBackToFarthestPoint", PZ_SPAWN_TEST_FLAGS)
bool FSpawnFallbackFarthestTest::RunTest(const FString& Parameters)
{
	// Arrange
	FRandomStream Random(PZSpawnTest::Seed);
	constexpr double Unreachable = 100000.0;
	constexpr int32 Drones = 3;

	// Act
	const TArray<int32> Picks = PZSpawnRules::ChooseSpawnPoints(PZSpawnTest::Points, PZSpawnTest::Player, Unreachable, Drones, Random);

	// Assert
	TestTrue(TEXT("every drone uses the farthest point"), Picks == TArray<int32>({ 2, 2, 2 }));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnSpreadsBeforeReuseTest, "Protocol_Zero.Spawn.SpreadsAcrossPointsBeforeReuse", PZ_SPAWN_TEST_FLAGS)
bool FSpawnSpreadsBeforeReuseTest::RunTest(const FString& Parameters)
{
	// Arrange
	FRandomStream Random(PZSpawnTest::Seed);
	constexpr int32 TwoDrones = 2; // two eligible points

	// Act
	const TArray<int32> Picks = PZSpawnRules::ChooseSpawnPoints(PZSpawnTest::Points, PZSpawnTest::Player, PZSpawnTest::MinDistance, TwoDrones, Random);

	// Assert
	TestTrue(TEXT("both eligible points used once"), Picks.Num() == 2 && Picks[0] != Picks[1]);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnSameSeedSameResultTest, "Protocol_Zero.Spawn.SameSeedGivesSamePicks", PZ_SPAWN_TEST_FLAGS)
bool FSpawnSameSeedSameResultTest::RunTest(const FString& Parameters)
{
	// Arrange
	FRandomStream First(PZSpawnTest::Seed);
	FRandomStream Second(PZSpawnTest::Seed);
	constexpr int32 Drones = 7;

	// Act
	const TArray<int32> A = PZSpawnRules::ChooseSpawnPoints(PZSpawnTest::Points, PZSpawnTest::Player, PZSpawnTest::MinDistance, Drones, First);
	const TArray<int32> B = PZSpawnRules::ChooseSpawnPoints(PZSpawnTest::Points, PZSpawnTest::Player, PZSpawnTest::MinDistance, Drones, Second);

	// Assert
	TestTrue(TEXT("deterministic for a seed"), A == B);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnNoPointsTest, "Protocol_Zero.Spawn.NoPointsGivesNoPicks", PZ_SPAWN_TEST_FLAGS)
bool FSpawnNoPointsTest::RunTest(const FString& Parameters)
{
	// Arrange
	FRandomStream Random(PZSpawnTest::Seed);
	const TArray<FVector> None;

	// Act
	const TArray<int32> Picks = PZSpawnRules::ChooseSpawnPoints(None, PZSpawnTest::Player, PZSpawnTest::MinDistance, 3, Random);

	// Assert
	TestEqual(TEXT("no picks without spawn points"), Picks.Num(), 0);
	return true;
}

#undef PZ_SPAWN_TEST_FLAGS

#endif
