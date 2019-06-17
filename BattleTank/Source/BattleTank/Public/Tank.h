// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

//Forward Declarations
class UTankBarrel; 
class UTankTurret;
class UTankAimingComponent;
class AProjectile;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = Aiming)
	UTankAimingComponent * TankAimingComponent = nullptr;

public:	
	void AimAt(FVector AimLocation);

	UFUNCTION(BlueprintCallable, Category = Controls)
	void Fire();

private:

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	//TODO : Remove once firing is moved to AimingComponent
	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 4000.f; 

	//can only edit the Archetype with EditDefaultsOnly, so all Tanks will have the same value(i.e. you cannot edit a specific instance)
	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSeconds = 3;

	//Local barrel reference for spawing projectile
	UTankBarrel* Barrel = nullptr;

	double LastFireTime = 0;
};

