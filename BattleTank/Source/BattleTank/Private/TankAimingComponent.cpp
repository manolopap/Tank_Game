// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"



// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet) {
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt( FVector AimLocation, float LaunchSpeed) {
	if (!ensure(Barrel)) { return; }
	auto BarrelLocation = Barrel->GetSocketLocation(FName("Projectile"));
	FVector LaunchVelocity;
	//Calculate LaunchVelocity(Out parameter from below)
	if (UGameplayStatics::SuggestProjectileVelocity
		(this, 
		 LaunchVelocity,
		 BarrelLocation,
		 AimLocation,
		 LaunchSpeed,
		 false, 
		 0,
		 0,
		 ESuggestProjVelocityTraceOption::DoNotTrace
		 )
		) 
	{	// Get the Unit vector of the velocity
		auto AimDirection = LaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		//MoveTurretTowards(AimDirection);
		//UE_LOG(LogTemp, Warning, TEXT("Aiming at %s"), *AimDirection.ToString());
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	//calculate difference between current barrel rotation and AimDirection rotation
	if (!ensure(Turret && Barrel)) { return; }
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	
	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.GetNormalized().Yaw);
}
/*
void UTankAimingComponent::MoveTurretTowards(FVector AimDirection) {
	if (!Turret) { return; }
	//calculate difference between current turret rotation and AimDirection rotation
	auto TurretRotator = Turret->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - TurretRotator;

	Turret->Rotate(DeltaRotator.GetNormalized().Yaw);
}
*/


