// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTAIMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTTAIMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTAIMoveTo();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector MoveToSelector;
	uint8 DistanceType;

};
