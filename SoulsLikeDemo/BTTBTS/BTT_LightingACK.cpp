// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LightingACK.h"
#include "AIController.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"

UBTT_LightingACK::UBTT_LightingACK()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_LightingACK::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AITemp = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AITemp && BBComp)
	{
		AITemp->ActivateNiagaraComp();
		AITemp->SpawnLightingCheckBox();
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTT_LightingACK::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AITemp = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AITemp && !AITemp->GetLightingACKBoolean())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
