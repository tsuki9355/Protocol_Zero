// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Waves/PZWaveData.h"
#include "Waves/PZWaveSchedule.h"
#include "Enemies/PZDrone.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZWaveTest
{
	constexpr double WaveStart = 10.0;
	constexpr float Gap = 4.f;
	constexpr double Tiny = 0.001;
	const TArray<float> ThreeGroups = { 0.f, Gap, Gap }; // release at +0, +4, +8
}

#define PZ_WAVE_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveFirstGroupAtStartTest, "Protocol_Zero.Waves.FirstGroupReleasesAtStart", PZ_WAVE_TEST_FLAGS)
bool FWaveFirstGroupAtStartTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZWaveSchedule Schedule(PZWaveTest::ThreeGroups);
	Schedule.Start(PZWaveTest::WaveStart);
	TArray<int32> Due;

	// Act
	Schedule.ConsumeDueGroups(PZWaveTest::WaveStart, Due);

	// Assert
	TestEqual(TEXT("groups released at start"), Due.Num(), 1);
	TestTrue(TEXT("first group released"), Due.Num() == 1 && Due[0] == 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveNextGroupWaitsTest, "Protocol_Zero.Waves.NextGroupWaitsForItsDelay", PZ_WAVE_TEST_FLAGS)
bool FWaveNextGroupWaitsTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZWaveSchedule Schedule(PZWaveTest::ThreeGroups);
	Schedule.Start(PZWaveTest::WaveStart);
	TArray<int32> Due;
	Schedule.ConsumeDueGroups(PZWaveTest::WaveStart, Due);
	Due.Reset();

	// Act
	const int32 Early = Schedule.ConsumeDueGroups(PZWaveTest::WaveStart + PZWaveTest::Gap - PZWaveTest::Tiny, Due);
	const int32 OnTime = Schedule.ConsumeDueGroups(PZWaveTest::WaveStart + PZWaveTest::Gap, Due);

	// Assert
	TestEqual(TEXT("nothing just before the delay"), Early, 0);
	TestEqual(TEXT("second group at its delay"), OnTime, 1);
	TestTrue(TEXT("it is group 1"), Due.Num() == 1 && Due[0] == 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveLongFrameInOrderTest, "Protocol_Zero.Waves.LongFrameReleasesGroupsInOrder", PZ_WAVE_TEST_FLAGS)
bool FWaveLongFrameInOrderTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZWaveSchedule Schedule(PZWaveTest::ThreeGroups);
	Schedule.Start(PZWaveTest::WaveStart);
	TArray<int32> Due;

	// Act
	Schedule.ConsumeDueGroups(PZWaveTest::WaveStart + 100.0, Due);

	// Assert
	TestTrue(TEXT("all groups released once, in order"), Due == TArray<int32>({ 0, 1, 2 }));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveFinishedAfterLastTest, "Protocol_Zero.Waves.FinishedOnlyAfterLastGroup", PZ_WAVE_TEST_FLAGS)
bool FWaveFinishedAfterLastTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZWaveSchedule Schedule(PZWaveTest::ThreeGroups);
	Schedule.Start(PZWaveTest::WaveStart);
	TArray<int32> Due;
	const double LastRelease = PZWaveTest::WaveStart + Schedule.GetReleaseOffset(2);

	// Act
	Schedule.ConsumeDueGroups(LastRelease - PZWaveTest::Tiny, Due);
	const bool bFinishedBefore = Schedule.IsFinished();
	Schedule.ConsumeDueGroups(LastRelease, Due);

	// Assert
	TestFalse(TEXT("not finished before the last group"), bFinishedBefore);
	TestTrue(TEXT("finished after the last group"), Schedule.IsFinished());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveNotStartedReleasesNothingTest, "Protocol_Zero.Waves.NotStartedReleasesNothing", PZ_WAVE_TEST_FLAGS)
bool FWaveNotStartedReleasesNothingTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZWaveSchedule Schedule(PZWaveTest::ThreeGroups);
	TArray<int32> Due;

	// Act
	const int32 Released = Schedule.ConsumeDueGroups(PZWaveTest::WaveStart + 100.0, Due);

	// Assert
	TestEqual(TEXT("no release before Start"), Released, 0);
	TestFalse(TEXT("not finished before Start"), Schedule.IsFinished());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveDataTotalsTest, "Protocol_Zero.Waves.DataTotalsAndDelays", PZ_WAVE_TEST_FLAGS)
bool FWaveDataTotalsTest::RunTest(const FString& Parameters)
{
	// Arrange
	UPZWaveData* Wave = NewObject<UPZWaveData>();
	Wave->SpawnGroups = {
		{ APZDrone::StaticClass(), 3, 0.f },
		{ APZDrone::StaticClass(), 2, PZWaveTest::Gap },
	};

	// Act
	const int32 Total = Wave->GetTotalDroneCount();
	const TArray<float> Delays = Wave->GetGroupDelays();

	// Assert
	TestEqual(TEXT("total drones"), Total, 5);
	TestTrue(TEXT("delays in order"), Delays == TArray<float>({ 0.f, PZWaveTest::Gap }));
	return true;
}

#undef PZ_WAVE_TEST_FLAGS

#endif
