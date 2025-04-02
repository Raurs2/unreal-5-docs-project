// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnWithCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APawnWithCamera::APawnWithCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//create components
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	
	//attach components
	RootComponent = MeshComp;
	SpringArmComp->SetupAttachment(MeshComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//assign spring arm properties
	SpringArmComp->SetRelativeLocationAndRotation(FVector(.0f, .0f, 50.0f), FRotator(-60.0f, .0f, .0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.f;

	//possess default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APawnWithCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnWithCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//handle camera zoom
	if (bZoomingIn)
	{
		ZoomFactor += DeltaTime / .5f;
	}
	else
	{
		ZoomFactor -= DeltaTime / .25f;
	}
	ZoomFactor = FMath::Clamp(ZoomFactor, .0f, 1.f);

	//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
	CameraComp->FieldOfView = FMath::Lerp(90.f, 60.f, ZoomFactor);
	SpringArmComp->TargetArmLength = FMath::Lerp(400.f, 300.f, ZoomFactor);

	//rotate our camera's yaw
	FRotator NewYaw = GetActorRotation();
	NewYaw.Yaw += CameraInput.X;
	SetActorRotation(NewYaw);

	//rotate our camera's pitch
	FRotator NewPitch = SpringArmComp->GetComponentRotation();
	NewPitch.Pitch = FMath::Clamp(NewPitch.Pitch + CameraInput.Y, -80.f, -15.f);
	SpringArmComp->SetWorldRotation(NewPitch);

	//handle movement
	if (!MovementInput.IsZero())
	{
		MovementInput = MovementInput.GetSafeNormal() * 100.f;
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
		NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void APawnWithCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawnWithCamera::ZoomIn);
	PlayerInputComponent->BindAction("ZoomIn", IE_Released, this, &APawnWithCamera::ZoomOut);

	PlayerInputComponent->BindAxis("MoveForward", this, &APawnWithCamera::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APawnWithCamera::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawnWithCamera::PitchCamera);
	PlayerInputComponent->BindAxis("Turn", this, &APawnWithCamera::YawCamera);


}

void APawnWithCamera::MoveForward(float AxisValue)
{
	//MovementInput.X = FMath::Clamp(AxisValue, -1.f, 1.f);
	MovementInput.X = AxisValue;
}

void APawnWithCamera::MoveRight(float AxisValue)
{
	//MovementInput.Y = FMath::Clamp(AxisValue, -1.f, 1.f);
	MovementInput.Y = AxisValue;
}

void APawnWithCamera::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APawnWithCamera::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void APawnWithCamera::ZoomIn()
{
	bZoomingIn = true;
}

void APawnWithCamera::ZoomOut()
{
	bZoomingIn = false;
}

