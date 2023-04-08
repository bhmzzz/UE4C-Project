// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_WF_ChoseToMove.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "BehaviorTree/BlackBoardComponent.h"


UBTT_WF_ChoseToMove::UBTT_WF_ChoseToMove()
{
}

EBTNodeResult::Type UBTT_WF_ChoseToMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		float Result = UKismetMathLibrary::RandomFloatInRange(1.f, 100.f);
		if (Result <= 20)
			BB->SetValueAsEnum(MoveToSelector.SelectedKeyName, 2);
		else if (Result > 20 && Result <= 60)
			BB->SetValueAsEnum(MoveToSelector.SelectedKeyName, 1);
		else if (Result > 60 && Result <= 100)
			BB->SetValueAsEnum(MoveToSelector.SelectedKeyName, 0);
		return EBTNodeResult::Succeeded;
	}
	else
		return EBTNodeResult::Failed;
}
