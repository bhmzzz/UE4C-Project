// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MyAISenseConfig_Sight.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UMyAISenseConfig_Sight : public UAISenseConfig_Sight
{
	GENERATED_BODY()
public:
	UMyAISenseConfig_Sight(const FObjectInitializer& ObjectInitializer);
	
};
