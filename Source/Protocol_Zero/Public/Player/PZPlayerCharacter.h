// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PZPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class UPZHealthComponent;

/**
 * The security operative. Moves freely on the ground under a fixed isometric camera
 * that never rotates with the character.
 */
UCLASS()
class PROTOCOL_ZERO_API APZPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APZPlayerCharacter();

	/** Moves on the ground plane relative to the camera. X is right, Y is forward. */
	void Move(const FVector2D& Input);

	/**
	 * Turns to face a world-space aim point on the ground plane and remembers it.
	 * Called every frame by the controller with the point under the mouse cursor.
	 */
	void AimAt(const FVector& WorldPoint);

	/** Last point the character aimed at, for the weapon to fire toward. */
	const FVector& GetAimPoint() const { return AimPoint; }

	/** Yaw of the fixed camera in degrees, used to make WASD camera-relative. */
	float GetCameraYaw() const { return CameraYaw; }

	/** Player health. */
	UPZHealthComponent* GetHealth() const { return Health; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Camera pitch in degrees. Negative looks down. */
	UPROPERTY(EditDefaultsOnly, Category = "Tuning|Camera")
	float CameraPitch = -45.f;

	/** Camera yaw in degrees. Fixed for the whole run. */
	UPROPERTY(EditDefaultsOnly, Category = "Tuning|Camera")
	float CameraYaw = -45.f;

	/** Distance from the character to the camera. */
	UPROPERTY(EditDefaultsOnly, Category = "Tuning|Camera", meta = (ClampMin = "100"))
	float CameraDistance = 1600.f;

	/** Camera field of view in degrees. */
	UPROPERTY(EditDefaultsOnly, Category = "Tuning|Camera", meta = (ClampMin = "10", ClampMax = "120"))
	float CameraFieldOfView = 55.f;

	/** Ground movement speed in cm/s. */
	UPROPERTY(EditDefaultsOnly, Category = "Tuning|Movement", meta = (ClampMin = "0"))
	float MoveSpeed = 600.f;

private:
	void ApplyTuning();

	FVector AimPoint = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	/** Greybox body. Replaced by the final character mesh in the art pass. */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Body;

	/** Greybox marker showing which way the character faces. */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> FacingMarker;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPZHealthComponent> Health;
};
