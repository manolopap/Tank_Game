// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack * LeftTrackToSet, UTankTrack* RightTrackToSet) {
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntentMoveForward(float Throw) {
	if (!ensure(LeftTrack && RightTrack)) { return; }
	//Give both tracks the same throttle 
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);

}

void UTankMovementComponent::IntentTurnRight(float Throw) {
	if (!ensure(LeftTrack && RightTrack)) { return; }
	//Give the tracks opposite throttle 
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed) {
	//Get a unit vector in the direction the tank is facing
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	//we only care about the direction of the velocity and not the magnitude, so we get the unit vector
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	//Need to adjust the angle of movement based on where the tank is facing and where it needs to go
	//So it uses the dot product of the two vectors to get the value of Throw.(Dot = |A|*|B|* cos(theta))
	//if the angle is < 90 i.e. at some point in front of the tank, then the Throw will be the value of cos(since the two vectors are unit), which is positive
	//Similarly, if the angle is >90 < 270 then the Throw will be negative.
	auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	IntentMoveForward(ForwardThrow);

	//For rotation, use the Cross Product with the same idea in mind
	//Cross Product uses sin, which means that if the two vectors are aligned, ie. 0 angle then the magnitude of the result(i.e. sine) will be 0
	//as there is no need to turn, etc.
	//We get the Z component which will mirror the throw.(i.e. if the vector is facing downwards then Z will be negative etc)
	auto RotationThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
	IntentTurnRight(RotationThrow);
}

