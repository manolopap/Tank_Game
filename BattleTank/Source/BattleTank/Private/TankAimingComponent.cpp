// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Engine/World.h"



// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}



void UTankAimingComponent::BeginPlay() {
	Super::BeginPlay();
	//Initial reload
	LastFireTime = GetWorld()->GetTimeSeconds();
}


void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet) {
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) {
	if (GetWorld()->GetTimeSeconds() - LastFireTime < ReloadTimeInSeconds) {
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving()) {
		FiringState = EFiringState::Aiming;
	}
	else {
		FiringState = EFiringState::Locked;
	}
}

bool UTankAimingComponent::IsBarrelMoving() {
	if (!ensure(Barrel)) { return false; }
	//if they are equal then the barrel is not moving
	return !(Barrel->GetForwardVector().Equals(AimDirection, 0.01));
}

void UTankAimingComponent::AimAt( FVector AimLocation) {
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
		AimDirection = LaunchVelocity.GetSafeNormal();
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

void UTankAimingComponent::Fire() {
	//if the barrel reference exists and the tank has reloaded, then fire
	if (FiringState != EFiringState::Reloading) {
		if (!ensure(Barrel && ProjectileBlueprint)) { return; }
		//Spawn a projectile at the socket location of the barrel
		AProjectile * Projectile = GetWorld()->SpawnActor<AProjectile>
			(ProjectileBlueprint,
				Barrel->GetSocketLocation(FName("Projectile")),
				Barrel->GetSocketRotation(FName("Projectile")));
		Projectile->LaunchProjectile(LaunchSpeed);
		//update lastFireTime
		LastFireTime = GetWorld()->GetTimeSeconds();
	}

}



