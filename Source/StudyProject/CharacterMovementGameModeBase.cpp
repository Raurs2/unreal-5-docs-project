// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMovementGameModeBase.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerCharacter.h"


ACharacterMovementGameModeBase::ACharacterMovementGameModeBase()
{

}

void ACharacterMovementGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &ACharacterMovementGameModeBase::PlayerDied);
	}
}

void ACharacterMovementGameModeBase::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void ACharacterMovementGameModeBase::PlayerDied(ACharacter* Character)
{
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);
}

