// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"


/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UMySaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float Score;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector PlayerLocation;
	
};
