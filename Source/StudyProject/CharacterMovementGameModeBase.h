// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CharacterMovementGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);

UCLASS()
class STUDYPROJECT_API ACharacterMovementGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ACharacterMovementGameModeBase();

	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }

	virtual void RestartPlayer(AController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void PlayerDied(ACharacter* Character);

	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;

	
};
