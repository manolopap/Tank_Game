// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack() {

	PrimaryComponentTick.bCanEverTick = false;
	
}

void UTankTrack::SetThrottle(float Throttle) {
	//Make sure Throttle doesnt go beyond 2 or -2(was meant to be -1 to 1 but was preventing 2 directions at once e.g forwrad and right, but -2 to 2 seems to work)
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -2, 2);
	//Drive tank
	DriveTrack(CurrentThrottle);
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const{
	TArray<ASprungWheel*> Wheels;
	TArray<USceneComponent*> Children;
	//get all the children of the track
	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children) {
		
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		//ignore non SpawnPoint children
		if (!SpawnPointChild) continue;
		//get the sprung wheel that was spawned there and add it to the array of wheels
		//should be 2 wheels per track
		ASprungWheel* SpawnedWheel = Cast<ASprungWheel>(SpawnPointChild->GetSpawnedActor());
		if (!SpawnedWheel) continue;
		Wheels.Add(SpawnedWheel);
	}
	return Wheels;
}

void UTankTrack::DriveTrack(float CurrentThrottle) {
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();
	for (ASprungWheel * Wheel : Wheels) {
		Wheel->AddDrivingForce(ForcePerWheel);
	}

	/*Old version without the physics constraint- adds a force at the location of the track in the direction that we want to go(forward, backward)
	//forward vector gives the direction of the force applied which is the same as the direction of the tank
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	//returns the component location of the track(can be seen when clicking on it in the viewport)
	auto ForceLocation = GetComponentLocation();
	//Get the tank body, by moving up to the highest level with GetOwner, and then getting the root component
	//since the highest level(scene component) does not allow for forces, we cast to primitive component
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);*/
}

/* Done more elegantly through the constraints, kept to have as an alternative solution
void UTankTrack::ApplySidewaysForce() {

	//Calculate slippage speed (the component of speed in the right direction : cosine of the angle between velocity and sliding vector)
	auto SlidingVectorUnit = GetRightVector();
	auto Velocity = GetComponentVelocity();
	auto SlippageSpeed = FVector::DotProduct(SlidingVectorUnit, Velocity);
	//Work out the required accelaration this frame to correct(a = s/t)
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	//Use GetRightVector() for the direction of the slippage and aply the acceleration on the opposite direction
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	//Calculate and apply sideways force(F = m * a)
	//Divide by 2 since there are 2 tracks
	auto CorrectionForce = TankRoot->GetMass() * CorrectionAcceleration / 2;
	TankRoot->AddForce(CorrectionForce);
}*/
