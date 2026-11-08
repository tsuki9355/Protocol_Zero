// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combat/PZProjectile.h"
#include "PZEnemyBolt.generated.h"

/**
 * Sentry projectile. Uses the EnemyProjectile collision profile, so it hits the player and
 * cover and passes through drones and other projectiles. Slower and larger than the player's
 * plasma bolt so it can be read and dodged.
 */
UCLASS()
class PROTOCOL_ZERO_API APZEnemyBolt : public APZProjectile
{
	GENERATED_BODY()

public:
	APZEnemyBolt();
};
