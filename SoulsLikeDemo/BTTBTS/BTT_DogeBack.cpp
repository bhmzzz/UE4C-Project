// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DogeBack.h"
#include "AIController.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"


UBTT_DogeBack::UBTT_DogeBack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_DogeBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC  = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (ensure(AI))
	{
		AI->AIDogeBack();
		AI->bCurrMontagePlay = true;
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTT_DogeBack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if(AI && !AI->bCurrMontagePlay)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
