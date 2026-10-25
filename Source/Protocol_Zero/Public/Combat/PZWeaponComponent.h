// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Combat/PZFireCadence.h"
#include "PZWeaponComponent.generated.h"

class UPZWeaponData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPZOnFired, FVector, MuzzleLocation, FVector, Direction);

/**
 * Automatic weapon. Fires toward the owner's aim point while the trigger is held,
 * one shot per cooldown. Place the component where bolts should leave the barrel.
 */
UCLASS(ClassGroup = (ProtocolZero), meta = (BlueprintSpawnableComponent))
class PROTOCOL_ZERO_API UPZWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPZWeaponComponent();

	/** Trigger pressed. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	/** Trigger released. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	/** World-space point the next shots travel toward. Set every frame by the owner. */
	void SetAimPoint(const FVector& InAimPoint) { AimPoint = InAimPoint; }

	/** Tuning asset for this weapon. */
	UPZWeaponData* GetWeaponData() const { return WeaponData; }

	/** Assigns the tuning asset. Call before BeginPlay, for example from the owner's constructor. */
	void SetWeaponData(UPZWeaponData* InWeaponData) { WeaponData = InWeaponData; }

	/** Fires once per bolt, after it spawns. */
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FPZOnFired OnFired;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/** Tuning asset: damage, cooldown and projectile class. */
	UPROPERTY(EditAnywhere, Category = "Tuning")
	TObjectPtr<UPZWeaponData> WeaponData;

	/** Greybox aid: draws a short-lived line per shot. Turn off once bolts are visible. */
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebugShots = true;

private:
	void FireShot();
	double GetNow() const;

	FPZFireCadence Cadence;
	FVector AimPoint = FVector::ZeroVector;
};
