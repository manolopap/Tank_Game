// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

//Depends on MovementComponent via Pathfinding System

void ATankAIController::BeginPlay() {

	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn* InPawn) {

	Super::SetPawn(InPawn);
	//if the pawn is possessed
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!PossessedTank) { return; }

		//Subscribe our local delegate method, to the Tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPawnDeath);
	}
}

void ATankAIController::OnPawnDeath() {

	GetPawn()->DetachFromControllerPendingDestroy();
}


void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();
	if (!(PlayerTank  && ControlledTank)) { return; }
	//Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius);
	//find the aiming component of the tank
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	//aim towards the player and fire
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	//if aiming or locked only, fire
	if (AimingComponent->GetFiringState() == EFiringState::Locked) {
		AimingComponent->Fire();
	}
	
	
}




