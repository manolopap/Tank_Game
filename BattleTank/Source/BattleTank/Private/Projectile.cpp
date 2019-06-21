// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/RadialForceComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Colision Mesh"));
	SetRootComponent(CollisionMesh);
	//if another class is made based on the Projectile class, this property will be set to true by default(in BP it is annotated as "Simulation Generates Hit Events")
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	//by default you dont want to see the body of the projectile(e.g. a big sphere), so you set it to false
	//can have it as true(here or in BP) for debugging and development purposes
	CollisionMesh->SetVisibility(false);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	//if you dont explicitly attach it will look like it's attached but might not actually be(same for other components)
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Movement Component"));
	//Prevents projectile from moving until we fire it off
	ProjectileMovementComponent->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//ExplosionForce->bIgnoreOwningActor = true;
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//projectile is an actor so we run the onComponentHit when the StaticMesh/CollisionMesh Hits something
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
}

void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit) {
	//When the projectile hits something, deactive the launch blast and activate the impact blast particle
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	//Add the impulse from the impact to the turret
	ExplosionForce->FireImpulse();
	//Destroy the collision mesh so that the projectile disappears after impact
	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	//Apply damage to the tank
	UGameplayStatics::ApplyRadialDamage(
		this,
		AttackDamage,
		GetActorLocation(),
		ExplosionForce->Radius, //set the damage radius as the radius of the explosion
		UDamageType::StaticClass(), //returns an object representing this class
		TArray<AActor*> () //give empty array of actors to damage = damage all actors within the radius

		);

	//Set a timer to destroy the projectile instance
	//Alternate ways to do this is: a) Use the Lifespan property
	//b)Instead of spawing and destroying with timer use a fixed amount of projectiles in the world("pool")
	//place them in a ring buffer and circle around them, so as to avoid spawing countless projectiles
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AProjectile::OnTimerExpire, DestroyDelay, false);
}

void AProjectile::LaunchProjectile(float Speed) {
	//Set the direction of the velocity as the forward vector of the projectile i.e. the same direction the barrel is facing
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovementComponent->Activate();
}

void AProjectile::OnTimerExpire() {
	Destroy();
}
