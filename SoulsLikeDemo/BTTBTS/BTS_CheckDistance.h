﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTS_CheckDistance : public UBTService
{
	GENERATED_BODY()
public:
		UBTS_CheckDistance();
		virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector DistanceSelector;

};
