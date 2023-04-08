﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossDied.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTT_BossDied : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_BossDied();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
