// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Combat/PZFireCadence.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZCadenceTest
{
	constexpr float Cooldown = 0.1f;
	constexpr double FrameTime = 1.0 / 60.0;
	// Ticks 0..59 cover [0, 0.983] s: shots due at 0.0, 0.1, ... 0.9.
	constexpr int32 TicksJustUnderOneSecond = 60;
	constexpr int32 ShotsInOneSecond = 10;
}

#define PZ_CADENCE_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCadencePullFiresImmediatelyTest, "Protocol_Zero.Weapon.PullingTriggerFiresImmediately", PZ_CADENCE_TEST_FLAGS)
bool FCadencePullFiresImmediatelyTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZFireCadence Cadence(PZCadenceTest::Cooldown);

	// Act
	Cadence.PullTrigger(0.0);
	const int32 Shots = Cadence.ConsumeDueShots(0.0);

	// Assert
	TestEqual(TEXT("shots on press"), Shots, 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCadenceHoldMatchesRateTest, "Protocol_Zero.Weapon.HoldingFiresAtCooldownRate", PZ_CADENCE_TEST_FLAGS)
bool FCadenceHoldMatchesRateTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZFireCadence Cadence(PZCadenceTest::Cooldown);
	Cadence.PullTrigger(0.0);
	int32 Shots = 0;

	// Act
	for (int32 Tick = 0; Tick < PZCadenceTest::TicksJustUnderOneSecond; ++Tick)
	{
		Shots += Cadence.ConsumeDueShots(Tick * PZCadenceTest::FrameTime);
	}

	// Assert
	TestEqual(TEXT("shots in one held second"), Shots, PZCadenceTest::ShotsInOneSecond);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCadenceLongFrameCatchesUpTest, "Protocol_Zero.Weapon.LongFrameDoesNotLoseShots", PZ_CADENCE_TEST_FLAGS)
bool FCadenceLongFrameCatchesUpTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr double LongFrameEnd = 0.35;
	constexpr int32 ExpectedShots = 4; // 0.0, 0.1, 0.2, 0.3
	FPZFireCadence Cadence(PZCadenceTest::Cooldown);
	Cadence.PullTrigger(0.0);

	// Act
	const int32 Shots = Cadence.ConsumeDueShots(0.0) + Cadence.ConsumeDueShots(LongFrameEnd);

	// Assert
	TestEqual(TEXT("shots across one long frame"), Shots, ExpectedShots);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCadenceTapCannotBeatCooldownTest, "Protocol_Zero.Weapon.RepressInsideCooldownDoesNotFire", PZ_CADENCE_TEST_FLAGS)
bool FCadenceTapCannotBeatCooldownTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr double RepressTime = 0.05;
	FPZFireCadence Cadence(PZCadenceTest::Cooldown);
	Cadence.PullTrigger(0.0);
	Cadence.ConsumeDueShots(0.0);
	Cadence.ReleaseTrigger();

	// Act
	Cadence.PullTrigger(RepressTime);
	const int32 Shots = Cadence.ConsumeDueShots(RepressTime);

	// Assert
	TestEqual(TEXT("no shot before the cooldown ends"), Shots, 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCadenceReleasedStopsTest, "Protocol_Zero.Weapon.ReleasedTriggerStopsFiring", PZ_CADENCE_TEST_FLAGS)
bool FCadenceReleasedStopsTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr double MuchLater = 5.0;
	FPZFireCadence Cadence(PZCadenceTest::Cooldown);
	Cadence.PullTrigger(0.0);
	Cadence.ConsumeDueShots(0.0);
	Cadence.ReleaseTrigger();

	// Act
	const int32 Shots = Cadence.ConsumeDueShots(MuchLater);

	// Assert
	TestEqual(TEXT("no shots while released"), Shots, 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCadenceIdleBanksNoBurstTest, "Protocol_Zero.Weapon.IdleTimeBanksNoBurst", PZ_CADENCE_TEST_FLAGS)
bool FCadenceIdleBanksNoBurstTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr double MuchLater = 5.0;
	FPZFireCadence Cadence(PZCadenceTest::Cooldown);

	// Act
	Cadence.PullTrigger(MuchLater);
	const int32 Shots = Cadence.ConsumeDueShots(MuchLater);

	// Assert
	TestEqual(TEXT("one shot after a long idle"), Shots, 1);
	return true;
}

#undef PZ_CADENCE_TEST_FLAGS

#endif
