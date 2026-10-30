// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "UI/PZHudFormat.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZHudTest
{
	constexpr float MaxHealth = 100.f;
}

#define PZ_HUD_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHudShowsCurrentOverMaxTest, "Protocol_Zero.Hud.ShowsCurrentOverMax", PZ_HUD_TEST_FLAGS)
bool FHudShowsCurrentOverMaxTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr float Current = 90.f;

	// Act
	const FString Text = PZHudFormat::HealthText(Current, PZHudTest::MaxHealth).ToString();

	// Assert
	TestEqual(TEXT("health text"), Text, FString(TEXT("90 / 100")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHudSliverReadsAliveTest, "Protocol_Zero.Hud.SliverOfHealthNeverReadsZero", PZ_HUD_TEST_FLAGS)
bool FHudSliverReadsAliveTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr float Sliver = 0.2f;

	// Act
	const FString Text = PZHudFormat::HealthText(Sliver, PZHudTest::MaxHealth).ToString();

	// Assert
	TestEqual(TEXT("a living player shows at least 1"), Text, FString(TEXT("1 / 100")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHudClampsOutOfRangeTest, "Protocol_Zero.Hud.ClampsOutOfRangeHealth", PZ_HUD_TEST_FLAGS)
bool FHudClampsOutOfRangeTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr float Negative = -5.f;
	constexpr float Overheal = 140.f;

	// Act
	const FString Low = PZHudFormat::HealthText(Negative, PZHudTest::MaxHealth).ToString();
	const FString High = PZHudFormat::HealthText(Overheal, PZHudTest::MaxHealth).ToString();

	// Assert
	TestEqual(TEXT("negative clamps to zero"), Low, FString(TEXT("0 / 100")));
	TestEqual(TEXT("above max clamps to max"), High, FString(TEXT("100 / 100")));
	return true;
}

#undef PZ_HUD_TEST_FLAGS

#endif
