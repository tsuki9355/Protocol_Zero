// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Combat/PZMeleeAttack.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZMeleeTest
{
	constexpr float Range = 150.f;
	constexpr float Cooldown = 1.f;
	constexpr double InRange = 100.0;
	constexpr double OutOfRange = 151.0;
}

#define PZ_MELEE_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMeleeHitsInRangeTest, "Protocol_Zero.Melee.HitsTargetInRange", PZ_MELEE_TEST_FLAGS)
bool FMeleeHitsInRangeTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZMeleeAttack Melee(PZMeleeTest::Range, PZMeleeTest::Cooldown);

	// Act
	const bool bHit = Melee.TryAttack(0.0, PZMeleeTest::InRange);

	// Assert
	TestTrue(TEXT("attack lands in range"), bHit);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMeleeOutOfRangeMissesTest, "Protocol_Zero.Melee.OutOfRangeNeverHits", PZ_MELEE_TEST_FLAGS)
bool FMeleeOutOfRangeMissesTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZMeleeAttack Melee(PZMeleeTest::Range, PZMeleeTest::Cooldown);

	// Act
	const bool bHit = Melee.TryAttack(0.0, PZMeleeTest::OutOfRange);

	// Assert
	TestFalse(TEXT("no hit out of range"), bHit);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMeleeCooldownBlocksTest, "Protocol_Zero.Melee.CooldownBlocksRepeatHit", PZ_MELEE_TEST_FLAGS)
bool FMeleeCooldownBlocksTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZMeleeAttack Melee(PZMeleeTest::Range, PZMeleeTest::Cooldown);
	Melee.TryAttack(0.0, PZMeleeTest::InRange);

	// Act
	const bool bHit = Melee.TryAttack(PZMeleeTest::Cooldown * 0.5, PZMeleeTest::InRange);

	// Assert
	TestFalse(TEXT("no hit while cooling down"), bHit);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMeleeHitsAfterCooldownTest, "Protocol_Zero.Melee.HitsAgainAfterCooldown", PZ_MELEE_TEST_FLAGS)
bool FMeleeHitsAfterCooldownTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZMeleeAttack Melee(PZMeleeTest::Range, PZMeleeTest::Cooldown);
	Melee.TryAttack(0.0, PZMeleeTest::InRange);

	// Act
	const bool bHit = Melee.TryAttack(PZMeleeTest::Cooldown, PZMeleeTest::InRange);

	// Assert
	TestTrue(TEXT("hit once the cooldown ends"), bHit);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMeleeMissDoesNotStartCooldownTest, "Protocol_Zero.Melee.MissDoesNotStartCooldown", PZ_MELEE_TEST_FLAGS)
bool FMeleeMissDoesNotStartCooldownTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr double JustAfter = 0.1;
	FPZMeleeAttack Melee(PZMeleeTest::Range, PZMeleeTest::Cooldown);
	Melee.TryAttack(0.0, PZMeleeTest::OutOfRange);

	// Act
	const bool bHit = Melee.TryAttack(JustAfter, PZMeleeTest::InRange);

	// Assert
	TestTrue(TEXT("closing in after a miss attacks at once"), bHit);
	return true;
}

#undef PZ_MELEE_TEST_FLAGS

#endif
