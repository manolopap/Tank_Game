// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"



ATank * ATankPlayerController::GetControlledTank() const {
	//get the pawn the player is currently possessing	
	return  Cast<ATank>(GetPawn());
}

void ATankPlayerController::BeginPlay() {

	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not possessing any tank"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Player Controller possessing: %s"), *(ControlledTank->GetName()));
	}
	//UE_LOG(LogTemp, Warning, TEXT("Player Controller Begin Play"));
}

void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//aim towards the UI crosshair at every tick(i.e. frame)
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair() {

	if (!GetControlledTank()) { return; }
	FVector HitLocation; // an OUT parameter
	//Get world location of linetrace through crosshair
	//if ray trace hits
	if (GetSightRayHitLocation(HitLocation)) {
		//TODO: tell controlled tank to aim at this point
	}
	//else
		
}
/*
* Ray-trace. If hit the landscape update the out parameter with the location and return true
*/
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const{
	//raycast into landscape
	auto result = FVector(1.0);//GetLocationHit();
	if (result != FVector(0,0, -999)) {
		OutHitLocation = result;
		return true;
	}
	return false;
	
	//if hit
		//get location of hit on landscape
		//update OutHitLocation
		//return true
	//else 
		//return false
	
}


 FVector ATankPlayerController::GetLocationHit() {
	///Setup query parameters
	///If we want to use complex tracing rather than simple we change the boolean to true
	///Ignore Owner so that the line trace will ignore us(otherwise the pawn will be the first object hit)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	///Line trace (aka Ray-cast) to reach distance
	FHitResult HitResult;
	///if a hit is found, return the result
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		ECollisionChannel(ECollisionChannel::ECC_WorldStatic),
		TraceParams)) 
	{
		return HitResult.Location;
	}
	return FVector(0, 0, -9999);
	
	//return HitResult;
}

FVector ATankPlayerController::GetReachLineStart() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FVector ATankPlayerController::GetReachLineEnd() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}