// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"

UTankTrack::UTankTrack() {

	PrimaryComponentTick.bCanEverTick = false;
	
}

void UTankTrack::BeginPlay() {
	Super::BeginPlay();
	//registers a callback to the OnHit function once a Hit is noticed(need to have "Simulation Generate Hit Event" turned on in the Blueprint)
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

/*
*Since OnHit is called every frame when the tracks are touching the ground, we can use that instead of Tick to perform movement calculations
*This also means that no forces will be computed when the tank is in the air.
*/
void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit) {
	//Drive tank
	DriveTrack();
	//Apply sideways force
	ApplySidewaysForce();
	//Reset throttle so that tank stops moving
	CurrentThrottle = 0;
}


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
}

void UTankTrack::SetThrottle(float Throttle) {
	//Make sure Throttle doesnt go beyond 2 or -2(was meant to be -1 to 1 but was preventing 2 directions at once e.g forwrad and right, but -2 to 2 seems to work)
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -2, 2);
}

void UTankTrack::DriveTrack() {
	//forward vector gives the direction of the force applied which is the same as the direction of the tank
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	//returns the component location of the track(can be seen when clicking on it in the viewport)
	auto ForceLocation = GetComponentLocation();
	//Get the tank body, by moving up to the highest level with GetOwner, and then getting the root component
	//since the highest level(scene component) does not allow for forces, we cast to primitive component
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

