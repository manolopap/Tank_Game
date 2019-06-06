// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"


ATank * ATankAIController::GetControlledTank() const {
	//get the pawn the player is currently possessing	
	return  Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const {
	auto playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!playerPawn) {
		return nullptr;	
	}
	return Cast<ATank>(playerPawn);
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//aim towards the UI crosshair at every tick(i.e. frame)
	AimTowardsPlayer();
}

void ATankAIController::AimTowardsPlayer() {

	if (!GetControlledTank() || !GetPlayerTank()) { return; }

	auto PlayerLocation = GetPlayerTank()->GetActorLocation();
	//TODO: Move towards the player
	//Aim towards the player location
	GetControlledTank()->AimAt(PlayerLocation);
	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at player location : %s"), *GetName(), *PlayerLocation.ToString());
}

void ATankAIController::BeginPlay() {

	Super::BeginPlay();
	//Find and log the tank possessed by the AI
	/*auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("AI Controller not possessing any tank"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AI Controller possessing: %s"), *(ControlledTank->GetName()));
	}*/
	//Find and log the Tank possessed by the First Player
	auto playerTank = GetPlayerTank();
	if (!playerTank) {
		UE_LOG(LogTemp, Warning, TEXT("No tanks possessed by player found"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AI Controller found player : %s"), *(playerTank->GetName()));
	}
	//UE_LOG(LogTemp, Warning, TEXT("Player Controller Begin Play"));
}


