// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "Engine/World.h"

//elevate it to that location this frame, given frame time and max elevation speed
void UTankBarrel::Elevate(float RelativeSpeed) {
	//clamp Relative speed to a factor between -1 and 1
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	//multiply the weighted(i.e. multiplied by the relative speed factor) rate of change with the time to get the value of change in degrees
	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	//the new raw elevation(i.e. not considering the max or min)
	float RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	float ClampedElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
	//UE_LOG(LogTemp, Warning, TEXT("Elevation: %f"), ClampedElevation)
	//elevate the barrel(i.e. change yaw) by the amount of elevation
	SetRelativeRotation(FRotator(ClampedElevation, 0, 0));
}


