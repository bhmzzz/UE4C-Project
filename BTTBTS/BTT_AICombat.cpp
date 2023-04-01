// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AICombat.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyController.h"
#include "AIController.h"

UBTT_AICombat::UBTT_AICombat()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_AICombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AI)
	{
		AI->PlayComboMontage();
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTT_AICombat::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ANormalEnemyController* AIC = Cast<ANormalEnemyController>(OwnerComp.GetAIOwner());
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AI->GetCurrentMode() == ENormalEnemyMode::ELockPlayer)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
