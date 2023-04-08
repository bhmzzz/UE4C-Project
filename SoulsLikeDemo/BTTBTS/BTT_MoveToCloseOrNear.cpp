// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToCloseOrNear.h"
#include "AIController.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_MoveToCloseOrNear::UBTT_MoveToCloseOrNear()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_MoveToCloseOrNear::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AITemp = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AIC && AITemp && BBComp)
	{
		UObject* TargetObject = BBComp->GetValueAsObject(FName("TargetActor"));//获取到黑板中的值
		playerActor = Cast<AActor>(TargetObject);
		if (ProbabilityFUNC(50.f))
			bMoveToClose = true;
		else
			bMoveToClose = false;
		AIC->MoveToActor(playerActor);
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Type();
}

void UBTT_MoveToCloseOrNear::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AITemp = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	FVector Target = playerActor->GetActorLocation();
	float dis = AITemp->GetDisWithVector(Target);
	if (bMoveToClose)
	{
		if (dis <= 190.f)
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		if (dis <= 240.f)
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
