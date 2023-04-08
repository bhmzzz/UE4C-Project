// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	AMyGameMode();
protected:
	virtual void BeginPlay() override;

	EGAME_MODE::Type CurrentGameMode;

public:
	EGAME_MODE::Type GetCurrentGameMode();
	void SetCurrentGameMode(EGAME_MODE::Type ChangeMode);
};
