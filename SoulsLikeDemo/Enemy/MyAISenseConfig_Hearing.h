// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "MyAISenseConfig_Hearing.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UMyAISenseConfig_Hearing : public UAISenseConfig_Hearing
{
	GENERATED_BODY()
public:
	UMyAISenseConfig_Hearing(const FObjectInitializer& ObjectInitializer);
};
