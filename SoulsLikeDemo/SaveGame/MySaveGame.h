// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
private:
	UMySaveGame();
	FVector PlayerLocation;
	static UMySaveGame* SaveGameIns;

public:
	void SavePlayerLocation(FVector location);
	static UMySaveGame* GetSaveGameIns();
	FVector GetPlayerLocation();
	
};
