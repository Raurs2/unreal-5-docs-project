// Fill out your copyright notice in the Description page of Project Settings.


#include "FireEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AFireEffect::AFireEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleFireComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleFireComponent"));
	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));

	ParticleFireComponent->SetupAttachment(RootComponent);
	FireAudioComponent->AttachToComponent(ParticleFireComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Tags.Add(FName("FindActorTag"));

}

// Called when the game starts or when spawned
void AFireEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

