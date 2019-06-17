// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"

//Depends on MovementComponent via Pathfinding System

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto ControlledTank = Cast<ATank>(GetPawn());
	if (PlayerTank) {
		//Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);
		//aim towards the player at every tick(i.e. frame)
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
		ControlledTank->Fire(); //TODO: Limit fire rate
	}
	
}

void ATankAIController::BeginPlay() {

	Super::BeginPlay();
}


