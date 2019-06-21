// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}



// Called when the game starts or when spawned
void ATank::BeginPlay() {

	Super::BeginPlay();

	

}

float ATank::GetHealthPercent() const{
	return (float)CurrentHealth / (float) StartingHealth ;
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) {
	//Do not want to compare float to 0(due to equality problems of floats) and thus we round to int
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	//Clamp the Actual Damage applied to the current Health(i.e cannot go below 0 health)
	int32 DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);
	//UE_LOG(LogTemp, Warning, TEXT("Damage Amount :%f, Damage To Apply: %i, CurrentHealth: %i"), DamageAmount, DamageToApply, CurrentHealth)
	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0) {
		OnDeath.Broadcast();
	}
	return DamageToApply;
}


