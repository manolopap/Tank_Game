// Copyright Emmanouil Papageorgiou and EmbraceIT Ltd

#include "SpawnPoint.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USpawnPoint::USpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	//spawn the actor with a delay at the transform of the component it will be attached to
	SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, GetComponentTransform());
	if (!SpawnedActor) { return; }
	//attach the new actor to the instance of this SpawnPoint at the same WolrdTransform
	//(if we used KeepRelative Transform as in other cases then we would have double the transform, due to adding one above)
	SpawnedActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	//complete the deferred spawning
	UGameplayStatics::FinishSpawningActor(SpawnedActor, GetComponentTransform());
	
}


// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


