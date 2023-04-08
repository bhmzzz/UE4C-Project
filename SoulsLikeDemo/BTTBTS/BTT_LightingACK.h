// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_LightingACK.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTT_LightingACK : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_LightingACK();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
