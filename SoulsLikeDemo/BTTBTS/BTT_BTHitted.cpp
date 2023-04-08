// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BTHitted.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCHaracter.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_BTHitted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AI)
	{
		AI->SetHittedValue(false);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
