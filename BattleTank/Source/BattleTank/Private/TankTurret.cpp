// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

//Rotate the turret to that location this frame, given frame time and max rotation speed
void UTankTurret::Rotate(float RelativeSpeed) {
	//clamp Relative speed to a factor between -1 and 1
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	//multiply the weighted(i.e. multiplied by the relative speed factor) rate of change with the time to get the value of change in degrees
	auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	//the new raw elevation(i.e. not considering the max or min)
	float NewRotation = RelativeRotation.Yaw + RotationChange;
	//float ClampedRotation = FMath::Clamp<float>(RawNewRotations, MinElevationDegrees, MaxElevationDegrees);
	//UE_LOG(LogTemp, Warning, TEXT("Rotation: %f"), NewRotation)
	//elevate the barrel(i.e. change yaw) by the amount of elevation
	SetRelativeRotation(FRotator(0, NewRotation, 0));
}


