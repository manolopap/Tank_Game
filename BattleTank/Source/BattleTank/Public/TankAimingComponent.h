// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

//Enum for the different aiming states
//Note that for Unreal enums you have to annotate with UENUM(), and also provide the general type you want to use(usually uint8)
UENUM()
enum class EFiringState : uint8
{
	Locked,
	Aiming,
	Reloading
};

//Forward declaration
class UTankBarrel;
class UTankTurret;
class AProjectile;

//
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();


public:	
	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	//Aims at the passed location with the given speed
	void AimAt(FVector AimLocation);


	UFUNCTION(BlueprintCallable, Category = Controls)
	void Fire();

protected:
	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringState FiringState = EFiringState::Reloading;

private:

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 4000.f;

	//can only edit the Archetype with EditDefaultsOnly, so all Tanks will have the same value(i.e. you cannot edit a specific instance)
	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSeconds = 3;

	double LastFireTime = 0;

	//get the barrel i.e. the start location of the projectile
	UTankBarrel* Barrel = nullptr;

	UTankTurret* Turret = nullptr;

	void MoveBarrelTowards(FVector AimDirection);

};
