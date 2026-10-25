// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PZWeaponData.generated.h"

/**
 * Tuning for one weapon. The starting plasma rifle is DA_PlasmaRifle in /Game/Data/Weapons.
 * Values are placeholders until the tuning pass; the design leaves exact numbers open.
 */
UCLASS(BlueprintType)
class PROTOCOL_ZERO_API UPZWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Damage dealt by each bolt. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = "0"))
	float Damage = 10.f;

	/** Seconds between shots while the trigger is held. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = "0.01"))
	float FireCooldown = 0.1f;

	/** Actor spawned for each bolt. Empty until the projectile story lands. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass;
};
