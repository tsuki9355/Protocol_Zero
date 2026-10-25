// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Combat/PZHealthComponent.h"
#include "Combat/PZProjectile.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/ProjectileMovementComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace PZProjectileTest
{
	constexpr float BoltDamage = 10.f;
	constexpr float TargetMaxHealth = 100.f; // UPZHealthComponent default
	constexpr float FrameTime = 1.f / 60.f;
	constexpr int32 FramesToSimulate = 60; // one second: bolt travels ~2500 cm
	constexpr double TargetDistance = 800.0;
	constexpr double CoverDistance = 400.0;
	const FVector BoxExtent(50.0, 200.0, 200.0);

	/** Minimal map-less game world with physics. Destroyed by the destructor. */
	struct FTestWorld
	{
		UWorld* World = nullptr;

		FTestWorld()
		{
			World = UWorld::CreateWorld(EWorldType::Game, false);
			FWorldContext& Context = GEngine->CreateNewWorldContext(EWorldType::Game);
			Context.SetCurrentWorld(World);
			World->InitializeActorsForPlay(FURL());
			World->BeginPlay();
		}

		~FTestWorld()
		{
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(false);
		}

		/** Spawns a box blocker at X with the given collision profile, optionally with health. */
		AActor* SpawnBox(double X, FName Profile, bool bWithHealth) const
		{
			AActor* Actor = World->SpawnActor<AActor>(AActor::StaticClass(), FTransform(FVector(X, 0.0, 0.0)));
			UBoxComponent* Box = NewObject<UBoxComponent>(Actor);
			Box->SetBoxExtent(BoxExtent);
			Box->SetCollisionProfileName(Profile);
			Actor->SetRootComponent(Box);
			Box->RegisterComponent();
			Actor->SetActorLocation(FVector(X, 0.0, 0.0));

			if (bWithHealth)
			{
				UPZHealthComponent* Health = NewObject<UPZHealthComponent>(Actor);
				Health->RegisterComponent();
				Health->ResetHealth();
			}
			return Actor;
		}

		/** Fires a bolt from the origin along +X. */
		TWeakObjectPtr<APZProjectile> FireBolt() const
		{
			const FTransform Transform(FRotator::ZeroRotator, FVector::ZeroVector);
			APZProjectile* Bolt = World->SpawnActorDeferred<APZProjectile>(APZProjectile::StaticClass(), Transform);
			Bolt->SetDamage(BoltDamage);
			Bolt->FinishSpawning(Transform);
			return Bolt;
		}

		/** Steps every live bolt's movement directly: with no game mode the world never begins play, so actor ticks do not run. */
		void Simulate() const
		{
			for (int32 Frame = 0; Frame < FramesToSimulate; ++Frame)
			{
				for (TActorIterator<APZProjectile> It(World); It; ++It)
				{
					if (IsValid(*It))
					{
						It->GetMovement()->TickComponent(FrameTime, LEVELTICK_All, nullptr);
					}
				}
			}
		}
	};

	float HealthOf(const AActor* Actor)
	{
		return Actor->FindComponentByClass<UPZHealthComponent>()->GetCurrentHealth();
	}
}

#define PZ_PROJECTILE_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectileDamagesEnemyTest, "Protocol_Zero.Projectile.PlayerBoltDamagesEnemy", PZ_PROJECTILE_TEST_FLAGS)
bool FProjectileDamagesEnemyTest::RunTest(const FString& Parameters)
{
	// Arrange
	PZProjectileTest::FTestWorld Test;
	AActor* Enemy = Test.SpawnBox(PZProjectileTest::TargetDistance, TEXT("Enemy"), true);
	const TWeakObjectPtr<APZProjectile> Bolt = Test.FireBolt();

	// Act
	Test.Simulate();

	// Assert
	TestEqual(TEXT("enemy health"), PZProjectileTest::HealthOf(Enemy), PZProjectileTest::TargetMaxHealth - PZProjectileTest::BoltDamage);
	TestFalse(TEXT("bolt destroyed on hit"), Bolt.IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectileStoppedByCoverTest, "Protocol_Zero.Projectile.CoverBlocksPlayerBolt", PZ_PROJECTILE_TEST_FLAGS)
bool FProjectileStoppedByCoverTest::RunTest(const FString& Parameters)
{
	// Arrange
	PZProjectileTest::FTestWorld Test;
	Test.SpawnBox(PZProjectileTest::CoverDistance, TEXT("BlockAll"), false);
	AActor* Enemy = Test.SpawnBox(PZProjectileTest::TargetDistance, TEXT("Enemy"), true);
	const TWeakObjectPtr<APZProjectile> Bolt = Test.FireBolt();

	// Act
	Test.Simulate();

	// Assert
	TestEqual(TEXT("enemy behind cover untouched"), PZProjectileTest::HealthOf(Enemy), PZProjectileTest::TargetMaxHealth);
	TestFalse(TEXT("bolt destroyed by cover"), Bolt.IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectileIgnoresPlayerTest, "Protocol_Zero.Projectile.PlayerBoltPassesThroughPlayer", PZ_PROJECTILE_TEST_FLAGS)
bool FProjectileIgnoresPlayerTest::RunTest(const FString& Parameters)
{
	// Arrange
	PZProjectileTest::FTestWorld Test;
	AActor* PlayerStandIn = Test.SpawnBox(PZProjectileTest::CoverDistance, TEXT("Pawn"), true);
	AActor* Enemy = Test.SpawnBox(PZProjectileTest::TargetDistance, TEXT("Enemy"), true);
	Test.FireBolt();

	// Act
	Test.Simulate();

	// Assert
	TestEqual(TEXT("player untouched"), PZProjectileTest::HealthOf(PlayerStandIn), PZProjectileTest::TargetMaxHealth);
	TestEqual(TEXT("enemy behind the player is hit"), PZProjectileTest::HealthOf(Enemy), PZProjectileTest::TargetMaxHealth - PZProjectileTest::BoltDamage);
	return true;
}

#undef PZ_PROJECTILE_TEST_FLAGS

#endif
