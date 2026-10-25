// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Combat/PZHealthState.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZHealthTest
{
	constexpr float MaxHealth = 100.f;
	constexpr float GraceSeconds = 0.5f;
	constexpr float Hit = 25.f;
}

#define PZ_HEALTH_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthDamageReducesHealthTest, "Protocol_Zero.Health.DamageReducesHealth", PZ_HEALTH_TEST_FLAGS)
bool FHealthDamageReducesHealthTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZHealthState Health(PZHealthTest::MaxHealth, PZHealthTest::GraceSeconds);

	// Act
	const EPZDamageResult Result = Health.ApplyDamage(PZHealthTest::Hit, 0.0);

	// Assert
	TestTrue(TEXT("result is Damaged"), Result == EPZDamageResult::Damaged);
	TestEqual(TEXT("health after one hit"), Health.GetCurrentHealth(), PZHealthTest::MaxHealth - PZHealthTest::Hit);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthHitInsideGraceIgnoredTest, "Protocol_Zero.Health.HitInsideGraceWindowIsIgnored", PZ_HEALTH_TEST_FLAGS)
bool FHealthHitInsideGraceIgnoredTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZHealthState Health(PZHealthTest::MaxHealth, PZHealthTest::GraceSeconds);
	Health.ApplyDamage(PZHealthTest::Hit, 0.0);

	// Act
	const EPZDamageResult Result = Health.ApplyDamage(PZHealthTest::Hit, PZHealthTest::GraceSeconds * 0.5);

	// Assert
	TestTrue(TEXT("result is Ignored"), Result == EPZDamageResult::Ignored);
	TestEqual(TEXT("health unchanged"), Health.GetCurrentHealth(), PZHealthTest::MaxHealth - PZHealthTest::Hit);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthHitAfterGraceAppliesTest, "Protocol_Zero.Health.HitAfterGraceWindowApplies", PZ_HEALTH_TEST_FLAGS)
bool FHealthHitAfterGraceAppliesTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZHealthState Health(PZHealthTest::MaxHealth, PZHealthTest::GraceSeconds);
	Health.ApplyDamage(PZHealthTest::Hit, 0.0);

	// Act
	const EPZDamageResult Result = Health.ApplyDamage(PZHealthTest::Hit, PZHealthTest::GraceSeconds);

	// Assert
	TestTrue(TEXT("result is Damaged"), Result == EPZDamageResult::Damaged);
	TestEqual(TEXT("health after two hits"), Health.GetCurrentHealth(), PZHealthTest::MaxHealth - 2.f * PZHealthTest::Hit);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthLethalHitKillsOnceTest, "Protocol_Zero.Health.LethalHitKillsExactlyOnce", PZ_HEALTH_TEST_FLAGS)
bool FHealthLethalHitKillsOnceTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZHealthState Health(PZHealthTest::MaxHealth, 0.f);

	// Act
	const EPZDamageResult First = Health.ApplyDamage(PZHealthTest::MaxHealth * 2.f, 0.0);
	const EPZDamageResult Second = Health.ApplyDamage(PZHealthTest::Hit, 1.0);

	// Assert
	TestTrue(TEXT("first lethal hit is Killed"), First == EPZDamageResult::Killed);
	TestTrue(TEXT("hit on a dead pool is Ignored"), Second == EPZDamageResult::Ignored);
	TestEqual(TEXT("health clamps at zero"), Health.GetCurrentHealth(), 0.f);
	TestTrue(TEXT("is dead"), Health.IsDead());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthNonPositiveDamageIgnoredTest, "Protocol_Zero.Health.NonPositiveDamageIsIgnored", PZ_HEALTH_TEST_FLAGS)
bool FHealthNonPositiveDamageIgnoredTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZHealthState Health(PZHealthTest::MaxHealth, PZHealthTest::GraceSeconds);

	// Act
	const EPZDamageResult Result = Health.ApplyDamage(0.f, 0.0);

	// Assert
	TestTrue(TEXT("result is Ignored"), Result == EPZDamageResult::Ignored);
	TestFalse(TEXT("zero damage starts no grace window"), Health.IsInvulnerable(0.0));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthResetRestoresTest, "Protocol_Zero.Health.ResetRestoresFullHealth", PZ_HEALTH_TEST_FLAGS)
bool FHealthResetRestoresTest::RunTest(const FString& Parameters)
{
	// Arrange
	FPZHealthState Health(PZHealthTest::MaxHealth, PZHealthTest::GraceSeconds);
	Health.ApplyDamage(PZHealthTest::MaxHealth, 0.0);

	// Act
	Health.Reset();

	// Assert
	TestEqual(TEXT("health"), Health.GetCurrentHealth(), PZHealthTest::MaxHealth);
	TestFalse(TEXT("not dead"), Health.IsDead());
	TestFalse(TEXT("grace window cleared"), Health.IsInvulnerable(0.0));
	return true;
}

#undef PZ_HEALTH_TEST_FLAGS

#endif
