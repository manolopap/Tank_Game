// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

//Forward Declaration
class ATank;
class UTankAimingComponent;
/**
 * Responsible for helping the player aim
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override; //the override keyword makes sure there is something to override in some predecessor

protected:
	UFUNCTION(BlueprintCallable, Category = Setup)
	ATank * GetControlledTank() const; //const means it doesnt change any member variables of the class

	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
	void FoundAimingComponent(UTankAimingComponent* AimComp); //move the access of the AimingComponent from the Tank class to here

private :
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 10000.f;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.3333;

	//Override the Tick function
	virtual void Tick(float DeltaTime) override;

	//aims the barrel where the crosshair is pointing.A shot would land where the crosshair intersects the world
	void AimTowardsCrosshair();
	
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	//finds the location where the line trace of the crosshair hits
	//returns true if something was hit and false otherwise
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;


};
