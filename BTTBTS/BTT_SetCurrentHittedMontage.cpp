// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetCurrentHittedMontage.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyController.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "BehaviorTree/BlackBoardComponent.h"

UBTT_SetCurrentHittedMontage::UBTT_SetCurrentHittedMontage()
{
}

EBTNodeResult::Type UBTT_SetCurrentHittedMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	ANormalEnemyController* AIC = Cast<ANormalEnemyController>(OwnerComp.GetAIOwner());
	if (AIC && BB)
	{
		ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
		if (AI)
		{
			UAnimMontage* CurrentMontage = AI->GetHittedMontage();
			BB->SetValueAsObject(FName("CurrentHittedMontage"), CurrentMontage);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Aborted;
}
