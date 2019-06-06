// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"




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
	FVector HitLocation = FVector(0); // an OUT parameter
	//Get world location of linetrace through crosshair.
	if (GetSightRayHitLocation(HitLocation)) {
		// tell controlled tank to aim at this point
		GetControlledTank()->AimAt(HitLocation);

	}
	//else

}
/*
* Ray-trace. If hit the landscape update the out parameter with the location and return true
*/
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const {
	//Find crosshair location
	int32 viewportSizeX, viewportSizeY;
	GetViewportSize(viewportSizeX, viewportSizeY);
	//calculate the viewport coordinates of the crosshair
	FVector2D ScreenLocation = FVector2D(viewportSizeX * CrosshairXLocation, viewportSizeY * CrosshairYLocation);
	FHitResult HitResult;
	//Get the Hit result directly from the screen position
	if (GetHitResultAtScreenPosition(ScreenLocation, ECollisionChannel::ECC_WorldStatic, false, HitResult)) {
		OutHitLocation = HitResult.ImpactPoint;
		return true;
	}

	/*
	Another way to do it is to de-project and then perform a line trace
	FVector CameraWorldLocation, LookDirection; //two out parameters for the deprojection(the world location is not used)
	//De-project the screen position of the crosshair to a direction and a 3d position in the world
	if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection)) {
		//if the deprojection is successful perform the line trace
		//updates the result parameter and returns the result of the line-trace

		return (GetLookVectorHitLocation(LookDirection, OutHitLocation));
	}*/
	return false;

}


bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const {
	///Setup query parameters(optional)
	///If we want to use complex tracing rather than simple we change the boolean to true
	///Ignore Owner so that the line trace will ignore us(otherwise the pawn will be the first object hit)
	///FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	FHitResult HitResult;
	auto StartWorldLocation = PlayerCameraManager->GetCameraLocation();
	/// the ending location of the raycast
	auto EndWorldLocation = StartWorldLocation + LookDirection * LineTraceRange;
	///Line trace (aka Ray-cast) to reach distance
	///if line trace succeeds, return the location of the hit
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartWorldLocation,
		EndWorldLocation,
		ECollisionChannel(ECollisionChannel::ECC_Visibility) //hit anything that is visible
		//TraceParams optionally use collision parameters
	))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false; //line trace didnt succeed
}