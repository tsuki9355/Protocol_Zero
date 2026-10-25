// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/PZMovementMath.h"

FVector PZMovementMath::CameraRelativeDirection(const FVector2D& Input, float CameraYaw)
{
	const FRotator YawOnly(0.f, CameraYaw, 0.f);
	const FVector Forward = YawOnly.Vector();
	const FVector Right = FRotator(0.f, CameraYaw + 90.f, 0.f).Vector();

	return (Forward * Input.Y + Right * Input.X).GetClampedToMaxSize(1.f);
}
