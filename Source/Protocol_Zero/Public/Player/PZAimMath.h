// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Pure cursor-aim helpers, kept free of actors so they can be unit-tested. */
namespace PZAimMath
{
	/**
	 * Intersects a cursor ray with the horizontal plane at PlaneZ.
	 * Example: ProjectCursorToPlane(CamLoc, CamDir, Character.Z, AimPoint) for top-down aiming.
	 * @return False when the ray points level or upward and never meets the plane.
	 */
	PROTOCOL_ZERO_API bool ProjectCursorToPlane(const FVector& RayOrigin, const FVector& RayDirection, double PlaneZ, FVector& OutPoint);

	/**
	 * Yaw in degrees that faces From toward To on the ground plane.
	 * @return False when the points are closer than MinDistance horizontally, so the facing is left unchanged.
	 */
	PROTOCOL_ZERO_API bool YawToward(const FVector& From, const FVector& To, float& OutYaw, double MinDistance = 1.0);
}
