// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnWithCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APawnWithCamera::APawnWithCamera()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    // Create components
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    
    // Attach components
    RootComponent = MeshComp;
    SpringArmComp->SetupAttachment(MeshComp);
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    // Assign spring arm properties
    SpringArmComp->SetRelativeLocationAndRotation(FVector(.0f, .0f, 50.0f), FRotator(-30.0f, .0f, .0f));
    SpringArmComp->TargetArmLength = MinZoom;
    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->CameraLagSpeed = CameraLagSpeed;

    // Possess default player
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

    // Handle camera zoom
    if (bZoomingIn)
    {
        ZoomFactor += DeltaTime / ZoomInFactor;
    }
    else
    {
        ZoomFactor -= DeltaTime / ZoomOutFactor;
    }
    ZoomFactor = FMath::Clamp(ZoomFactor, .0f, 1.f);

    // Blend our camera's FOV and our SpringArm's length based on ZoomFactor
    CameraComp->FieldOfView = FMath::Lerp(MaxFov, MinFov, ZoomFactor);
    SpringArmComp->TargetArmLength = FMath::Lerp(MinZoom, MaxZoom, ZoomFactor);

    // Rotate our camera's yaw
    FRotator NewYaw = GetActorRotation();
    NewYaw.Yaw += CameraInput.X;
    SetActorRotation(NewYaw);

    // Rotate our camera's pitch
    FRotator NewPitch = SpringArmComp->GetComponentRotation();
    NewPitch.Pitch = FMath::Clamp(NewPitch.Pitch + CameraInput.Y, -80.f, -5.f);
    SpringArmComp->SetWorldRotation(NewPitch);

    // Handle movement
    if (!MovementInput.IsZero())
    {
        

        if (!bRunning)
        {
            MovementInput = MovementInput.GetSafeNormal() * PawnSpeed;
        }
        else
        {
            MovementInput = MovementInput.GetSafeNormal() * PawnRunSpeed;
        }
        FVector NewLocation = GetActorLocation();
        NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
        NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
        SetActorLocation(NewLocation);
    }

    if (bWobble && !MovementInput.IsZero())
    {
		float randomPitch = FMath::RandRange(-.5f, .5f);
		float randomYaw = FMath::RandRange(-.5f, .5f);
        float PitchWobble = FMath::Sin(GetWorld()->TimeSeconds * 3.f + randomPitch) * .1f;
        float YawWobble = FMath::Cos(GetWorld()->TimeSeconds * 3.f + randomYaw) * .1f;
        FRotator Wobble = SpringArmComp->GetComponentRotation();
        Wobble.Pitch += PitchWobble;
        Wobble.Yaw += YawWobble;
        SpringArmComp->SetWorldRotation(Wobble);
    }
    else
    {
        FRotator SpringArmRotation = SpringArmComp->GetComponentRotation();
        SpringArmRotation.Pitch = FMath::FInterpTo(SpringArmRotation.Pitch, -30.0f, DeltaTime, 2.5f);
        SpringArmRotation.Yaw = FMath::FInterpTo(SpringArmRotation.Yaw, 0.0f, DeltaTime, 2.5f);
        SpringArmComp->SetWorldRotation(SpringArmRotation);
        FRotator ActorRotation = GetActorRotation();
        ActorRotation.Yaw = FMath::FInterpTo(ActorRotation.Yaw, 0.0f, DeltaTime, 2.5f);
        SetActorRotation(ActorRotation);
    }
}

// Called to bind functionality to input
void APawnWithCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawnWithCamera::ZoomIn);
    PlayerInputComponent->BindAction("ZoomIn", IE_Released, this, &APawnWithCamera::ZoomOut);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APawnWithCamera::Run);

    PlayerInputComponent->BindAxis("MoveForward", this, &APawnWithCamera::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APawnWithCamera::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &APawnWithCamera::PitchCamera);
    PlayerInputComponent->BindAxis("Turn", this, &APawnWithCamera::YawCamera);
}

void APawnWithCamera::MoveForward(float AxisValue)
{
    MovementInput.X = AxisValue;
}

void APawnWithCamera::MoveRight(float AxisValue)
{
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

void APawnWithCamera::Run()
{
    bRunning = !bRunning;
}
