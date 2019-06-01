// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override; //the override keyword makes sure there is something to override in some predecessor
private :
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 10000.f;

	UPROPERTY(EditAnywhere)
	float CrosshairXLocation = 0.5;

	UPROPERTY(EditAnywhere)
	float CrosshairYLocation = 0.3333;

	ATank * GetControlledTank() const; //const means it doesnt change any member variables of the class

	//Override the Tick function
	virtual void Tick(float DeltaTime) override;

	//aims the barrel where the crosshair is pointing.A shot would land where the crosshair intersects the world
	void AimTowardsCrosshair();
	
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	//finds the location where the line trace of the crosshair hits
	//returns true if something was hit and false otherwise
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;


};
