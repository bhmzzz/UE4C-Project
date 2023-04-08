// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveToCloseOrNear.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTT_MoveToCloseOrNear : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_MoveToCloseOrNear();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
protected:
	bool bMoveToClose;
	AActor* playerActor;
};
