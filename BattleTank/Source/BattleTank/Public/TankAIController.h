// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

//Forward Declaration
class ATank;

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
	//Override the Tick function
	virtual void Tick(float DeltaTime) override;
};
