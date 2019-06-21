// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
	

public:
	FTankDelegate OnDeath;
	//Override the AActor take damage function. Called by the Engine when actor damage is dealt
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	//Return current health as a percentage of starting health, between 0 and 1
	UFUNCTION(BlueprintPure, Category = Health)
	float GetHealthPercent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Sets default values for this pawn's properties
	ATank();

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	int32 StartingHealth = 200;

	UPROPERTY(VisibleAnywhere, Category = Health)
	int32 CurrentHealth = StartingHealth;
};

/*
A delegate is like an event
To declare Dynamic Multicast Delegates
Eg in Tank
1.Create type: DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateType)
2.Declare a property of type FDelegateType: FDelegateType OnSomethingHappened;
3.Broadcast that property: OnSomethingHappened.Broadcast(); (e.g. in when CurrentHealth becomes 0)

On the receiver of the broadcast(eg. in TankAIController or TankPlayerController)
4.Declare a UFUNCTION delegate method that will handle the delegate when it gets called
UFUNCTION()
void DelegateMethod();
5.Register to listen(binds the delegate method to when the OnSomethingHappened property occurs):
Find possessed instance(i.e. possed tank): BroadcastingInstance
BroadcastingInstace->OnSomethingHappened.AddUniqueDynamic(this(i.e. the receiver), &AListenerClass::DelegateMethod(i.e. go find the delegate method that gets executed))

*/