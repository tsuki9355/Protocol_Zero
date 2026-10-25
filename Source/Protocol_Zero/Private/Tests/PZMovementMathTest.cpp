// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Player/PZMovementMath.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZMovementTest
{
	constexpr double Tolerance = 1.e-4;
	constexpr float IsometricYaw = -45.f;
	const FVector2D Forward(0.f, 1.f);
	const FVector2D Right(1.f, 0.f);
	const FVector2D ForwardRight(1.f, 1.f);
}

#define PZ_MOVEMENT_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMovementForwardFollowsCameraYawTest, "Protocol_Zero.Movement.ForwardFollowsCameraYaw", PZ_MOVEMENT_TEST_FLAGS)
bool FMovementForwardFollowsCameraYawTest::RunTest(const FString& Parameters)
{
	// Arrange
	const FVector Expected = FRotator(0.f, PZMovementTest::IsometricYaw, 0.f).Vector();

	// Act
	const FVector Direction = PZMovementMath::CameraRelativeDirection(PZMovementTest::Forward, PZMovementTest::IsometricYaw);

	// Assert
	TestTrue(TEXT("W moves along the camera's ground forward"), Direction.Equals(Expected, PZMovementTest::Tolerance));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMovementRightIsPerpendicularTest, "Protocol_Zero.Movement.RightIsCameraRight", PZ_MOVEMENT_TEST_FLAGS)
bool FMovementRightIsPerpendicularTest::RunTest(const FString& Parameters)
{
	// Arrange
	const FVector Expected = FRotator(0.f, PZMovementTest::IsometricYaw + 90.f, 0.f).Vector();

	// Act
	const FVector Direction = PZMovementMath::CameraRelativeDirection(PZMovementTest::Right, PZMovementTest::IsometricYaw);

	// Assert
	TestTrue(TEXT("D moves along the camera's ground right"), Direction.Equals(Expected, PZMovementTest::Tolerance));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMovementDiagonalNotFasterTest, "Protocol_Zero.Movement.DiagonalIsNotFaster", PZ_MOVEMENT_TEST_FLAGS)
bool FMovementDiagonalNotFasterTest::RunTest(const FString& Parameters)
{
	// Arrange (input set in the fixture)

	// Act
	const FVector Direction = PZMovementMath::CameraRelativeDirection(PZMovementTest::ForwardRight, PZMovementTest::IsometricYaw);

	// Assert
	TestEqual(TEXT("diagonal length"), Direction.Size(), 1.0, PZMovementTest::Tolerance);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMovementStaysOnGroundTest, "Protocol_Zero.Movement.StaysOnGroundPlane", PZ_MOVEMENT_TEST_FLAGS)
bool FMovementStaysOnGroundTest::RunTest(const FString& Parameters)
{
	// Arrange (input set in the fixture)

	// Act
	const FVector Direction = PZMovementMath::CameraRelativeDirection(PZMovementTest::ForwardRight, PZMovementTest::IsometricYaw);

	// Assert
	TestEqual(TEXT("no vertical component"), Direction.Z, 0.0, PZMovementTest::Tolerance);
	return true;
}

#undef PZ_MOVEMENT_TEST_FLAGS

#endif
