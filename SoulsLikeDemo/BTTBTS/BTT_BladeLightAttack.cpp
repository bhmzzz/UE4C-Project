// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BladeLightAttack.h"
#include "AIController.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"

UBTT_BladeLightAttack::UBTT_BladeLightAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_BladeLightAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AI)
	{
		AI->AIBladeLightAttack();
		AI->bCurrMontagePlay = true;
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTT_BladeLightAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AI && !AI->bCurrMontagePlay)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}






