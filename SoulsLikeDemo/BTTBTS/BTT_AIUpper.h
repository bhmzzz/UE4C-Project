// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_AIUpper.generated.h"

/**
 * 
 */

//���������Ϊ��AI�ڿ����ܻ�ʱ������ҵķ������������Ҹ���һ��LaunchCharacter()����ʹ�䲻�׵�����ʵ�ֿ�������

UCLASS()
class SOULSLIKEDEMO_API UBTT_AIUpper : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_AIUpper();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
