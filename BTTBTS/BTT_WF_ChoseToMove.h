// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_WF_ChoseToMove.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTT_WF_ChoseToMove : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_WF_ChoseToMove();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector MoveToSelector;
};
