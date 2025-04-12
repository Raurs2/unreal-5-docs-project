// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FireEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "HealthComponent.h"
#include "CharacterMovementGameModeBase.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Instantiating class Components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	// Set the location and rotation of the Character Mesh Transform
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FQuat(FRotator(0.f, -90.f, 0.f)));

	// Attaching classes Components
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	// Setting class variables of the spring arm
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->TargetArmLength = CameraDistance;


	bUseControllerRotationPitch = bDoPlayerRotateWithCamera;
	bUseControllerRotationYaw = bDoPlayerRotateWithCamera;
	bUseControllerRotationRoll = bDoPlayerRotateWithCamera;

	// Setting class variables of the Character movement component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCharacterMovement()->JumpZVelocity = JumpSpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	GetCharacterMovement()->AirControl = AirControl;
	GetCharacterMovement()->BrakingDecelerationWalking = BrakDecelerateWalk;
	GetCharacterMovement()->MinAnalogWalkSpeed = AnalogWalkSpeed;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnHealthChanged);
	
}

void APlayerCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health, float DamageAmount, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		Destroy();
	}
	UE_LOG(LogTemp, Warning, TEXT("The Player's Current Health is: %f"), Health);
}

void APlayerCharacter::DamagePlayerCharacter()
{
	UGameplayStatics::ApplyDamage(this, 20.f, GetInstigatorController(), this, GenericDamageType);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.f))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.f))
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
}

void APlayerCharacter::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::BeginCrouch()
{
	Crouch();
}

void APlayerCharacter::EndCrouch()
{
	UnCrouch();
}

void APlayerCharacter::FindActor()
{
	TArray<AActor*> ActorsToFind;

	if (UWorld* World = GetWorld())
	{
		//UGameplayStatics::GetAllActorsOfClass(World, AFireEffect::StaticClass(), ActorsToFind);
		UGameplayStatics::GetAllActorsOfClassWithTag(World, AFireEffect::StaticClass(), FName("FindActorTag"), ActorsToFind);
	}

	for (AActor* FireEffectAtor : ActorsToFind)
	{
		AFireEffect* FireEffectCast = Cast<AFireEffect>(FireEffectAtor);

		if (FireEffectCast)
		{
			FireEffectCast->GetParticleFireComponent()->Deactivate();
			FireEffectCast->GetFireAudioComponent()->Deactivate();
		}
	}
}

void APlayerCharacter::Destroyed()
{
	Super::Destroyed();
	if (UWorld* World = GetWorld())
	{
		if (ACharacterMovementGameModeBase* GameMode = Cast<ACharacterMovementGameModeBase>(World->GetAuthGameMode()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Broadcasting Player"));
			GameMode->GetOnPlayerDied().Broadcast(this);
		}
	}
}

void APlayerCharacter::CallRestartPlayer()
{
	AController* ControllerRef = GetController();

	Destroy();


	if (UWorld* World = GetWorld())
	{
		if (ACharacterMovementGameModeBase* GameMode = Cast<ACharacterMovementGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->RestartPlayer(ControllerRef);
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the movement functions to the input
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// Bind the camera rotation functions to the input
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);

	// Bind the sprint functions to the input
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::EndSprint);

	// Bind the crouch functions to the input
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::EndCrouch);

	// Bind the jump functions to the input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	// Bind the find actor function to the input
	PlayerInputComponent->BindAction("FindActor", IE_Pressed, this, &APlayerCharacter::FindActor);

	// Bind the damage function to the input
	PlayerInputComponent->BindAction("TakeDamage", IE_Pressed, this, &APlayerCharacter::DamagePlayerCharacter);

	// Bind the restart function to the input
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &APlayerCharacter::CallRestartPlayer);
}

