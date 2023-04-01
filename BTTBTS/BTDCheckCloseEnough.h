// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDCheckCloseEnough.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBTDCheckCloseEnough : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDCheckCloseEnough();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
protected:
	UPROPERTY(EditAnywhere, Category = LimitDis)
	float fDis;
};
