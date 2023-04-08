// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SelectMidACKType.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "BehaviorTree/BlackBoardComponent.h"

UBTT_SelectMidACKType::UBTT_SelectMidACKType()
{
}

EBTNodeResult::Type UBTT_SelectMidACKType::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (ensure(BB))
	{
		BB->SetValueAsBool(ACKTypeSelector.SelectedKeyName, ProbabilityFUNC(PrabilityValue));
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
