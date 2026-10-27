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
 * The default setup is the Hunter: DA_Hunter tuning and APZHunterAIController.
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

	/** Tuning asset for this drone. */
	UPZDroneData* GetDroneData() const { return DroneData; }

	/** Drone health. */
	UPZHealthComponent* GetHealth() const { return Health; }

	/** True from death until the destruction effect finishes and the drone despawns. */
	bool IsDying() const { return bDying; }

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void PostInitializeComponents() override;

	/** Tuning asset: health, speed and attack values. */
	UPROPERTY(EditAnywhere, Category = "Tuning")
	TObjectPtr<UPZDroneData> DroneData;

private:
	UFUNCTION()
	void HandleDeath();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPZHealthComponent> Health;

	FPZMeleeAttack Melee;

	bool bDying = false;
	float DyingElapsed = 0.f;
	FVector BodyBaseScale = FVector::OneVector;
};
