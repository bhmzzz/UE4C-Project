// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SelectMidACKType.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTT_SelectMidACKType : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SelectMidACKType();
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector ACKTypeSelector;
	UPROPERTY(EditAnywhere, Category = "AI")
		float PrabilityValue;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
