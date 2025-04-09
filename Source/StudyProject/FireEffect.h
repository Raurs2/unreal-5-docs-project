// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireEffect.generated.h"

UCLASS()
class STUDYPROJECT_API AFireEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireEffect();

	UParticleSystemComponent* GetParticleFireComponent() const { return ParticleFireComponent; }

	UAudioComponent* GetFireAudioComponent() const { return FireAudioComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystemComponent* ParticleFireComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAudioComponent* FireAudioComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
