// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetAIFocus.h"
#include "AIController.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"

UBTT_SetAIFocus::UBTT_SetAIFocus()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_SetAIFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
		if (AIC && Target && AI)
		{
			//设置AI聚焦于玩家
			AIC->SetFocus(Target);
			AI->SetFocusBoolean(true);
			AIC->GetBlackboardComponent()->SetValueAsBool("IsFocus", true);
			return EBTNodeResult::Succeeded;
		}
	return EBTNodeResult::Failed;
}

void UBTT_SetAIFocus::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

}
