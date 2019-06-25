// Copyright Emmanouil Papageorgiou and EmbraceIT Ltd

#include "SprungWheel.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/**
	Attachment hierarchy:
	1.Tank(through tank body)
	2.Mass Constraint
	3.Axle
	4.Axle-Wheel Constraint
	5.Wheel
	*/
	MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Mass-Wheel Constraint"));
	SetRootComponent(MassWheelConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	//different way of attaching to a component
	Axle->SetupAttachment(MassWheelConstraint);


	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Axle-Wheel Constraint"));
	AxleWheelConstraint->SetupAttachment(Axle);


}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	SetupConstraint();
	
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASprungWheel::AddDrivingForce(float ForceMagnitude) {

	FVector Force = Axle->GetForwardVector() * ForceMagnitude;
	Wheel->AddForce(Force);
}

void ASprungWheel::SetupConstraint() {
	auto ParentActor = GetAttachParentActor();
	//get the Actor to which suspension is attached i.e. the Tank
	if (!ParentActor) { return; }
	UPrimitiveComponent * BodyRoot = Cast<UPrimitiveComponent>(ParentActor->GetRootComponent());
	if (!BodyRoot) { return; }
	//get the root component i.e. the body of the tank, and the wheel, and bind them to the constraint
	MassWheelConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Cast<UPrimitiveComponent>(Axle), NAME_None);
	AxleWheelConstraint->SetConstrainedComponents(Cast<UPrimitiveComponent>(Axle), NAME_None, Cast<UPrimitiveComponent>(Wheel), NAME_None);
}

