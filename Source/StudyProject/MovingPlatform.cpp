// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FVector Destination = bMovedToTarget ? StartLocation : TargetLocation;
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, Destination, DeltaTime, Speed);
	SetActorLocation(NewLocation);
	if (FVector::Dist(NewLocation, Destination) < 1.f)
	{
		bMovedToTarget = !bMovedToTarget;
	}

}

