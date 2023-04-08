// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTT_SelectFarACKType.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTT_SelectFarACKType : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SelectFarACKType();
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector FarACKSelector;
	UPROPERTY(EditAnywhere, Category = "AI")
	float PrabilityValue;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
