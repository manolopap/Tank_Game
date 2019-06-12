// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"


void UTankTrack::SetThrottle(float Throttle) {

	//Clamp actual throttle so that player can't overdrive
	FMath::Clamp<float>(Throttle, -1, 1);
	//forward vector gives the direction of the force applied which is the same as the direction of the tank
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	//returns the component location of the track(can be seen when clicking on it in the viewport)
	auto ForceLocation = GetComponentLocation();
	//Get the tank body, by moving up to the highest level with GteOwner, and then getting the root component
	//since the highest level(scene component) does not allow for forces, we cast to primitive component
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

