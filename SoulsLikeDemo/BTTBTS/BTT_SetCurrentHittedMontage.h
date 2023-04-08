// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetCurrentHittedMontage.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTT_SetCurrentHittedMontage : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SetCurrentHittedMontage();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:

};
