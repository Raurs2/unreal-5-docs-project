// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnWithCamera.generated.h"

UCLASS()
class STUDYPROJECT_API APawnWithCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnWithCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	float ZoomInFactor = 0.5f;

	UPROPERTY(EditAnywhere)
	float ZoomOutFactor = 0.25f;

	UPROPERTY(EditAnywhere)
	float MaxZoom = 300.f;

	UPROPERTY(EditAnywhere)
	float MinZoom = 400.f;

	UPROPERTY(EditAnywhere)
	float MaxFov = 90.f;

	UPROPERTY(EditAnywhere)
	float MinFov = 60.f;

	UPROPERTY(EditAnywhere)
	float PawnSpeed = 200.f;

	UPROPERTY(EditAnywhere)
	float PawnRunSpeed = 400.f;

	UPROPERTY(EditAnywhere)
	float CameraLagSpeed = 3.f;

	UPROPERTY(EditAnywhere)
	bool bWobble = false;


	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;
	bool bRunning;
	bool bResetWobble = false;
	bool bResetTimer = true;
	FTimerHandle WobbleResetTimerHandle;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ZoomIn();
	void ZoomOut();
	void Run();
	void ResetWobble();
	void ResetTimer();

};
