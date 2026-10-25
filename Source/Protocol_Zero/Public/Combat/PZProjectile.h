// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PZProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;

/**
 * Visible energy bolt with finite travel time. Damages the first actor with a health
 * component it hits and is destroyed on any blocking hit.
 *
 * Who it can hit comes from the collision profile: "PlayerProjectile" (default) hits enemies
 * and cover; "EnemyProjectile" hits the player and cover. Projectiles never hit each other.
 *
 * Example:
 *   APZProjectile* Bolt = World->SpawnActorDeferred<APZProjectile>(Class, Transform, Owner);
 *   Bolt->SetDamage(10.f);
 *   Bolt->FinishSpawning(Transform);
 */
UCLASS()
class PROTOCOL_ZERO_API APZProjectile : public AActor
{
	GENERATED_BODY()

public:
	APZProjectile();

	/** Damage dealt on hit. Set before FinishSpawning. */
	void SetDamage(float InDamage) { Damage = InDamage; }

	/** Damage dealt on hit. */
	float GetDamage() const { return Damage; }

	/** Speed, lifetime and bounce settings for the bolt. */
	UProjectileMovementComponent* GetMovement() const { return Movement; }

protected:
	virtual void PostInitializeComponents() override;

private:
	UFUNCTION()
	void HandleStop(const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> Collision;

	/** Greybox visual. Replaced by the emissive bolt in the art pass. */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Visual;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> Movement;

	float Damage = 0.f;
};
