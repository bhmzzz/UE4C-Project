// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_AIUpper.generated.h"

/**
 * 
 */

//此类的作用为在AI在空中受击时候，向玩家的方向吸附，并且给予一个LaunchCharacter()这样使其不易掉落以实现空中连击

UCLASS()
class SOULSLIKEDEMO_API UBTT_AIUpper : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_AIUpper();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
