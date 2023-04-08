// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_JumpACK.h"
#include "AIController.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"

UBTT_JumpACK::UBTT_JumpACK()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_JumpACK::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (ensure(AI))
	{
		AI->AIJumpAttack();
		AI->bCurrMontagePlay = true;
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTT_JumpACK::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AI && !AI->bCurrMontagePlay)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
