// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"



// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet) {
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt( FVector AimLocation, float LaunchSpeed) {
	if (!Barrel) { return; }
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
			
		UE_LOG(LogTemp, Warning, TEXT("Aiming at %s"), *AimDirection.ToString());
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	//calculate difference between current barrel rotation and AimDirection rotation
	/*auto BarrelRotator = Barrel->GetForwardVector().Rotation;
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;*/
	
	Barrel->Elevate(5);
}




