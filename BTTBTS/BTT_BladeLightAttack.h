// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BladeLightAttack.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTT_BladeLightAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_BladeLightAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
};
