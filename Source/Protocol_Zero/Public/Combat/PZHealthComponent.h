// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/PZHealthState.h"
#include "PZHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPZOnDamaged, float, Amount, FVector, HitDirection, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPZOnDeath);

/**
 * Health for the player and drones. Handles damage, a post-hit invulnerability
 * window and a single death event. Tuning is set per Blueprint.
 */
UCLASS(ClassGroup = (ProtocolZero), meta = (BlueprintSpawnableComponent))
class PROTOCOL_ZERO_API UPZHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPZHealthComponent();

	/**
	 * Applies damage at the current world time. Ignored while dead or invulnerable.
	 * @param Amount Health to remove.
	 * @param HitDirection World-space direction the hit came from, for the hit indicator.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyDamage(float Amount, FVector HitDirection);

	/** Restores full health and clears the invulnerability window, for example on restart. */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ResetHealth();

	/** Health remaining. */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth() const { return State.GetCurrentHealth(); }

	/** Health at the start of each life. */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return State.GetMaxHealth(); }

	/** True once health has reached zero. */
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return State.IsDead(); }

	/** Fires after every hit that removes health, including the lethal one. */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FPZOnDamaged OnDamaged;

	/** Fires once, on the hit that takes health to zero. */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FPZOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;

	/** Health at the start of each life. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tuning", meta = (ClampMin = "1"))
	float MaxHealth = 100.f;

	/** Seconds after a hit during which further hits are ignored. Zero disables the window. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tuning", meta = (ClampMin = "0"))
	float InvulnerabilityDuration = 0.f;

private:
	FPZHealthState State;
};
