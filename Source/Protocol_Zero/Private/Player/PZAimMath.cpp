// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/PZAimMath.h"

bool PZAimMath::ProjectCursorToPlane(const FVector& RayOrigin, const FVector& RayDirection, double PlaneZ, FVector& OutPoint)
{
	if (RayDirection.Z >= -UE_KINDA_SMALL_NUMBER)
	{
		return false;
	}

	const double Distance = (PlaneZ - RayOrigin.Z) / RayDirection.Z;
	if (Distance < 0.0)
	{
		return false;
	}

	OutPoint = RayOrigin + RayDirection * Distance;
	return true;
}

bool PZAimMath::YawToward(const FVector& From, const FVector& To, float& OutYaw, double MinDistance)
{
	const FVector Flat(To.X - From.X, To.Y - From.Y, 0.0);
	if (Flat.SizeSquared() < MinDistance * MinDistance)
	{
		return false;
	}

	OutYaw = Flat.Rotation().Yaw;
	return true;
}
