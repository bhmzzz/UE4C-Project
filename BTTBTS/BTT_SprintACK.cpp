﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SprintACK.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTT_SprintACK::UBTT_SprintACK()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_SprintACK::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AI)
		AI->PlaySprintACKMontage();
	return EBTNodeResult::InProgress;
}

void UBTT_SprintACK::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AI->GetCurrentMode() == ENormalEnemyMode::ELockPlayer)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
