// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	
	virtual void BeginPlay() override; //the override keyword makes sure there is something to override in some predecessor

private:
	ATank * GetControlledTank() const; //const means it doesnt change any member variables of the class
	
	ATank * GetPlayerTank() const;
};