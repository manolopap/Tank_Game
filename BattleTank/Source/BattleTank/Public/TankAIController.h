// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

//Forward Declaration
class UTankAimingComponent;
class ATank;
/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	
	

protected:
	//How close can the AI tank get to the player
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float AcceptanceRadius = 8000;

private:
	virtual void BeginPlay() override; //the override keyword makes sure there is something to override in some predecessor
	//Override the Tick function
	virtual void Tick(float DeltaTime) override;
	
	//this gets called when the pawn is possessed
	virtual void SetPawn(APawn* InPawn) override;

	//Function to be called when the possessed tank dies
	UFUNCTION()
	void OnPawnDeath();
};
