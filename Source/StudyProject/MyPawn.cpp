// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include <Camera/CameraComponent.h>

// Sets default values
AMyPawn::AMyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create a camera and a visible object
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));

	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentScale = OurVisibleComponent->GetComponentScale().X;
	if (bGrowing)
	{
		CurrentScale += DeltaTime;
	}
	else
	{
		CurrentScale -= (DeltaTime * .5f);
	}

	CurrentScale = FMath::Clamp(CurrentScale, 1.f, 2.f);
	OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));

	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Grow", IE_Pressed, this, &AMyPawn::StartGrowing);
	PlayerInputComponent->BindAction("Grow", IE_Released, this, &AMyPawn::StopGrowing);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveXAxis);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveYAxis);

}

void AMyPawn::MoveXAxis(float AxisValue)
{
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.f, 1.f) * Speed;
}

void AMyPawn::MoveYAxis(float AxisValue)
{
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.f, 1.f) * Speed;
}

void AMyPawn::StartGrowing()
{
	bGrowing = true;
}

void AMyPawn::StopGrowing()
{
	bGrowing = false;
}

