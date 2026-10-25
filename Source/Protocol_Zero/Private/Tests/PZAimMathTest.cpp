// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Player/PZAimMath.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZAimTest
{
	constexpr double Tolerance = 1.e-3;
	constexpr double PlaneZ = 90.0;
	const FVector CameraLocation(-1000.0, -1000.0, 1500.0);
	const FVector Character(0.0, 0.0, PlaneZ);
}

#define PZ_AIM_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAimRayHitsPlaneTest, "Protocol_Zero.Aim.DownwardRayHitsAimPlane", PZ_AIM_TEST_FLAGS)
bool FAimRayHitsPlaneTest::RunTest(const FString& Parameters)
{
	// Arrange
	const FVector Target(300.0, 200.0, PZAimTest::PlaneZ);
	const FVector Direction = (Target - PZAimTest::CameraLocation).GetSafeNormal();
	FVector Point;

	// Act
	const bool bHit = PZAimMath::ProjectCursorToPlane(PZAimTest::CameraLocation, Direction, PZAimTest::PlaneZ, Point);

	// Assert
	TestTrue(TEXT("ray meets the plane"), bHit);
	TestTrue(TEXT("hit point is the cursor target"), Point.Equals(Target, PZAimTest::Tolerance));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAimUpwardRayMissesTest, "Protocol_Zero.Aim.UpwardRayMissesAimPlane", PZ_AIM_TEST_FLAGS)
bool FAimUpwardRayMissesTest::RunTest(const FString& Parameters)
{
	// Arrange
	const FVector Upward(0.0, 0.0, 1.0);
	FVector Point;

	// Act
	const bool bHit = PZAimMath::ProjectCursorToPlane(PZAimTest::CameraLocation, Upward, PZAimTest::PlaneZ, Point);

	// Assert
	TestFalse(TEXT("upward ray never meets the plane"), bHit);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAimYawFacesTargetTest, "Protocol_Zero.Aim.YawFacesTarget", PZ_AIM_TEST_FLAGS)
bool FAimYawFacesTargetTest::RunTest(const FString& Parameters)
{
	// Arrange
	const FVector TargetOnPositiveY(0.0, 500.0, PZAimTest::PlaneZ);
	float Yaw = 0.f;

	// Act
	const bool bAimed = PZAimMath::YawToward(PZAimTest::Character, TargetOnPositiveY, Yaw);

	// Assert
	TestTrue(TEXT("aim resolved"), bAimed);
	TestEqual(TEXT("yaw toward +Y"), Yaw, 90.f, 1.e-3f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAimIgnoresHeightTest, "Protocol_Zero.Aim.YawIgnoresHeightDifference", PZ_AIM_TEST_FLAGS)
bool FAimIgnoresHeightTest::RunTest(const FString& Parameters)
{
	// Arrange
	const FVector HighTargetOnNegativeX(-500.0, 0.0, PZAimTest::PlaneZ + 400.0);
	float Yaw = 0.f;

	// Act
	PZAimMath::YawToward(PZAimTest::Character, HighTargetOnNegativeX, Yaw);

	// Assert
	TestEqual(TEXT("yaw toward -X"), FMath::Abs(Yaw), 180.f, 1.e-3f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAimCursorOnCharacterKeepsFacingTest, "Protocol_Zero.Aim.CursorOnCharacterKeepsFacing", PZ_AIM_TEST_FLAGS)
bool FAimCursorOnCharacterKeepsFacingTest::RunTest(const FString& Parameters)
{
	// Arrange
	constexpr float PreviousYaw = 37.f;
	float Yaw = PreviousYaw;

	// Act
	const bool bAimed = PZAimMath::YawToward(PZAimTest::Character, PZAimTest::Character, Yaw);

	// Assert
	TestFalse(TEXT("no aim when cursor is on the character"), bAimed);
	TestEqual(TEXT("yaw untouched"), Yaw, PreviousYaw);
	return true;
}

#undef PZ_AIM_TEST_FLAGS

#endif
