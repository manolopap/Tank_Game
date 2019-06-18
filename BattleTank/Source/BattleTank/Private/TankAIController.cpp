// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"

//Depends on MovementComponent via Pathfinding System

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();
	if (!ensure(PlayerTank  && ControlledTank)) { return; }
	//Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius);
	//find the aiming component of the tank
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	//aim towards the player and fire
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	//if aiming or locked only, fire
	if (AimingComponent->GetFiringState() != EFiringState::Reloading) {
		AimingComponent->Fire();
	}
	
	
}

void ATankAIController::BeginPlay() {

	Super::BeginPlay();
}


