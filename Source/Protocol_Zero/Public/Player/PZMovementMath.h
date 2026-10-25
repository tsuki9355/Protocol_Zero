// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Pure movement helpers, kept free of actors so they can be unit-tested. */
namespace PZMovementMath
{
	/**
	 * Converts WASD input into a world-space ground direction relative to the camera's yaw.
	 * @param Input X is right (D positive), Y is forward (W positive).
	 * @param CameraYaw Camera yaw in degrees.
	 * @return Direction on the XY plane with length at most 1, so diagonals are not faster.
	 */
	PROTOCOL_ZERO_API FVector CameraRelativeDirection(const FVector2D& Input, float CameraYaw);
}
