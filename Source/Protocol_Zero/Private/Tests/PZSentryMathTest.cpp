// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Enemies/PZSentryMath.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZSentryTest
{
	constexpr float PreferredRange = 1000.f;
	constexpr float ResumeMargin = 150.f;
	constexpr double Inside = 900.0;
	constexpr double JustOutside = 1050.0;  // past preferred range, inside the margin
	constexpr double FarOutside = 1300.0;   // past preferred range plus the margin
}

#define PZ_SENTRY_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSentryApproachesWhenFarTest, "Protocol_Zero.Sentry.ApproachesWhenOutOfRange", PZ_SENTRY_TEST_FLAGS)
bool FSentryApproachesWhenFarTest::RunTest(const FString& Parameters)
{
	// Arrange (distances set in the fixture)

	// Act
	const EPZSentryAction Action = PZSentryMath::DecideAction(PZSentryTest::JustOutside, PZSentryTest::PreferredRange, PZSentryTest::ResumeMargin, true, false);

	// Assert
	TestTrue(TEXT("approaches from outside preferred range"), Action == EPZSentryAction::Approach);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSentryHoldsInRangeTest, "Protocol_Zero.Sentry.HoldsAndFiresInRange", PZ_SENTRY_TEST_FLAGS)
bool FSentryHoldsInRangeTest::RunTest(const FString& Parameters)
{
	// Arrange (distances set in the fixture)

	// Act
	const EPZSentryAction Action = PZSentryMath::DecideAction(PZSentryTest::Inside, PZSentryTest::PreferredRange, PZSentryTest::ResumeMargin, true, false);

	// Assert
	TestTrue(TEXT("holds and fires inside preferred range"), Action == EPZSentryAction::HoldAndFire);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSentryCoverForcesApproachTest, "Protocol_Zero.Sentry.BlockedShotForcesApproach", PZ_SENTRY_TEST_FLAGS)
bool FSentryCoverForcesApproachTest::RunTest(const FString& Parameters)
{
	// Arrange (distances set in the fixture)

	// Act
	const EPZSentryAction Action = PZSentryMath::DecideAction(PZSentryTest::Inside, PZSentryTest::PreferredRange, PZSentryTest::ResumeMargin, false, true);

	// Assert
	TestTrue(TEXT("repositions when cover blocks the shot, even in range"), Action == EPZSentryAction::Approach);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSentryMarginPreventsJitterTest, "Protocol_Zero.Sentry.HoldingSentryKeepsHoldingInsideMargin", PZ_SENTRY_TEST_FLAGS)
bool FSentryMarginPreventsJitterTest::RunTest(const FString& Parameters)
{
	// Arrange (distances set in the fixture)

	// Act
	const EPZSentryAction InsideMargin = PZSentryMath::DecideAction(PZSentryTest::JustOutside, PZSentryTest::PreferredRange, PZSentryTest::ResumeMargin, true, true);
	const EPZSentryAction PastMargin = PZSentryMath::DecideAction(PZSentryTest::FarOutside, PZSentryTest::PreferredRange, PZSentryTest::ResumeMargin, true, true);

	// Assert
	TestTrue(TEXT("keeps holding just past preferred range"), InsideMargin == EPZSentryAction::HoldAndFire);
	TestTrue(TEXT("repositions once past the margin"), PastMargin == EPZSentryAction::Approach);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSentryNeverRetreatsTest, "Protocol_Zero.Sentry.NeverRetreatsWhenPlayerCloses", PZ_SENTRY_TEST_FLAGS)
bool FSentryNeverRetreatsTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr double PointBlank = 50.0;

	// Act
	const EPZSentryAction Action = PZSentryMath::DecideAction(PointBlank, PZSentryTest::PreferredRange, PZSentryTest::ResumeMargin, true, true);

	// Assert
	TestTrue(TEXT("stays put and fires at point blank"), Action == EPZSentryAction::HoldAndFire);
	return true;
}

#undef PZ_SENTRY_TEST_FLAGS

#endif
