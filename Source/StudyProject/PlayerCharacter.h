// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class STUDYPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Spring Arm Component to follow the camera behind the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	// Camera Component to follow the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDoPlayerRotateWithCamera = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirControl = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnalogWalkSpeed = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakDecelerateWalk = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraDistance = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UHealthComponent* HealthComponent;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComp, float Health, float DamageAmount, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void DamagePlayerCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> GenericDamageType;


	// Handle forward/backward movement
	void MoveForward(float AxisValue);

	// Handle right/left movement
	void MoveRight(float AxisValue);

	void BeginSprint();

	void EndSprint();

	void BeginCrouch();

	void EndCrouch();

	void FindActor();

	virtual void Destroyed() override;

	void CallRestartPlayer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
