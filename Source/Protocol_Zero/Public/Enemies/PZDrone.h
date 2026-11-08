// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/PZMeleeAttack.h"
#include "PZDrone.generated.h"

class UPZDroneData;
class UPZHealthComponent;
class UStaticMeshComponent;

/**
 * Hostile drone. Walks the navmesh, takes damage from player bolts and attacks on a
 * cooldown. Behaviour comes from its AI controller; tuning comes from a UPZDroneData asset.
 * This class is the Hunter (melee, DA_Hunter, APZHunterAIController); APZSentryDrone is the
 * ranged variant.
 */
UCLASS()
class PROTOCOL_ZERO_API APZDrone : public ACharacter
{
	GENERATED_BODY()

public:
	APZDrone();

	/**
	 * Hits Target if it is in melee range and the cooldown has finished.
	 * @return True if the attack landed.
	 */
	bool TryMeleeAttack(AActor* Target);

	/** True if Target is close enough for a melee hit. */
	bool IsInMeleeRange(const AActor* Target) const;

	/**
	 * Fires the drone's projectile at Target if it is within attack range and the cooldown has
	 * finished. Shots use the enemy projectile profile, so they hit the player and cover only.
	 * @return True if a shot was fired.
	 */
	bool TryRangedAttack(AActor* Target);

	/** Tuning asset for this drone. */
	UPZDroneData* GetDroneData() const { return DroneData; }

	/** Drone health. */
	UPZHealthComponent* GetHealth() const { return Health; }

	/** True from death until the destruction effect finishes and the drone despawns. */
	bool IsDying() const { return bDying; }

	/**
	 * Scales this drone's base health and damage for the current wave and restores full health.
	 * Called by the wave manager right after spawning.
	 */
	void ApplyWaveMultipliers(float HealthMultiplier, float DamageMultiplier);

	/** Damage per attack after the wave multiplier. */
	float GetAttackDamage() const;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void PostInitializeComponents() override;

	/** Tuning asset: health, speed and attack values. */
	UPROPERTY(EditAnywhere, Category = "Tuning")
	TObjectPtr<UPZDroneData> DroneData;

	/** Greybox body. Subclasses swap the mesh to tell drone types apart. */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Body;

private:
	UFUNCTION()
	void HandleDeath();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPZHealthComponent> Health;

	/** Range and cooldown gate shared by melee and ranged attacks. */
	FPZMeleeAttack AttackGate;

	float WaveDamageMultiplier = 1.f;

	bool bDying = false;
	float DyingElapsed = 0.f;
	FVector BodyBaseScale = FVector::OneVector;
};
