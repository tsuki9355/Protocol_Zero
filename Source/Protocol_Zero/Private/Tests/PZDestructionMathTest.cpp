// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Enemies/PZDestructionMath.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZDestructionTest
{
	constexpr float Duration = 0.4f;
	constexpr float Tolerance = 1.e-4f;
}

#define PZ_DESTRUCTION_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDestructionStartsAtFullSizeTest, "Protocol_Zero.Destruction.StartsAtFullSize", PZ_DESTRUCTION_TEST_FLAGS)
bool FDestructionStartsAtFullSizeTest::RunTest(const FString& Parameters)
{
	// Arrange (duration set in the fixture)

	// Act
	const float Scale = PZDestructionMath::DestructionScale(0.f, PZDestructionTest::Duration);

	// Assert
	TestEqual(TEXT("scale at death"), Scale, 1.f, PZDestructionTest::Tolerance);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDestructionSwellsToPeakTest, "Protocol_Zero.Destruction.SwellsToPeak", PZ_DESTRUCTION_TEST_FLAGS)
bool FDestructionSwellsToPeakTest::RunTest(const FString& Parameters)
{
	// Arrange
	const float PeakTime = PZDestructionTest::Duration * PZDestructionMath::SwellFraction;

	// Act
	const float Scale = PZDestructionMath::DestructionScale(PeakTime, PZDestructionTest::Duration);

	// Assert
	TestEqual(TEXT("scale at the end of the swell"), Scale, PZDestructionMath::PeakScale, PZDestructionTest::Tolerance);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDestructionCollapsesToZeroTest, "Protocol_Zero.Destruction.CollapsesToZeroAtEnd", PZ_DESTRUCTION_TEST_FLAGS)
bool FDestructionCollapsesToZeroTest::RunTest(const FString& Parameters)
{
	// Arrange (duration set in the fixture)

	// Act
	const float AtEnd = PZDestructionMath::DestructionScale(PZDestructionTest::Duration, PZDestructionTest::Duration);
	const float Past = PZDestructionMath::DestructionScale(PZDestructionTest::Duration * 2.f, PZDestructionTest::Duration);

	// Assert
	TestEqual(TEXT("scale at the end"), AtEnd, 0.f, PZDestructionTest::Tolerance);
	TestEqual(TEXT("scale stays at zero after the end"), Past, 0.f, PZDestructionTest::Tolerance);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDestructionZeroDurationTest, "Protocol_Zero.Destruction.ZeroDurationVanishesAtOnce", PZ_DESTRUCTION_TEST_FLAGS)
bool FDestructionZeroDurationTest::RunTest(const FString& Parameters)
{
	// Arrange (no duration)

	// Act
	const float Scale = PZDestructionMath::DestructionScale(0.f, 0.f);

	// Assert
	TestEqual(TEXT("scale with no effect time"), Scale, 0.f, PZDestructionTest::Tolerance);
	return true;
}

#undef PZ_DESTRUCTION_TEST_FLAGS

#endif
