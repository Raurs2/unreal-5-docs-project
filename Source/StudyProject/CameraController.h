// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraController.generated.h"

UCLASS()
class STUDYPROJECT_API ACameraController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraController();

	UPROPERTY(EditAnywhere)
	AActor* CameraOne;

	UPROPERTY(EditAnywhere)
	AActor* CameraTwo;

	UPROPERTY(EditAnywhere)
	float TimeToNextCameraChange = 0.0f;

	UPROPERTY(EditAnywhere)
	float TimeBetweenCameraChanges = 2.0f;

	UPROPERTY(EditAnywhere)
	float SmoothBlendTime = 0.75f;

	UPROPERTY(EditAnywhere)
	APlayerController* OurPlayerController;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
