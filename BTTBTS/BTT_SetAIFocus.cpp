// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetAIFocus.h"
#include "AIController.h"
#include "BehaviorTree/BlackBoardComponent.h"

UBTT_SetAIFocus::UBTT_SetAIFocus()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_SetAIFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
		AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
		if (AIC && Target)
		{
			//设置AI聚焦于玩家
			AIC->SetFocus(Target);
			return EBTNodeResult::Succeeded;
		}
	return EBTNodeResult::Failed;
}

void UBTT_SetAIFocus::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

}
