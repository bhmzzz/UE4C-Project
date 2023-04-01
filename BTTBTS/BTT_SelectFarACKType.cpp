// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SelectFarACKType.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "BehaviorTree/BlackBoardComponent.h"

UBTT_SelectFarACKType::UBTT_SelectFarACKType()
{
}

EBTNodeResult::Type UBTT_SelectFarACKType::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (ensure(BB))
	{
		BB->SetValueAsBool(FarACKSelector.SelectedKeyName,ProbabilityFUNC(PrabilityValue));
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
