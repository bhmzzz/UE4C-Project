// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AIBeHitted.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

UBTT_AIBeHitted::UBTT_AIBeHitted()
{
	bNotifyTick = true;	//执行tickTask的必要设置
}

EBTNodeResult::Type UBTT_AIBeHitted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	ANormalEnemyController* AIController = Cast<ANormalEnemyController>(OwnerComp.GetAIOwner());
	UWorld* world = GetWorld();
	if (AIController && BB)
	{
		ANormalEnemyCharacter* AITemp = Cast<ANormalEnemyCharacter>(AIController->GetPawn());
		if (AITemp)
		{
			FRotator DifRotation = UKismetMathLibrary::FindLookAtRotation(AITemp->GetActorLocation(), UGameplayStatics::GetPlayerCharacter(world, 0)->GetActorLocation());
			FRotator newRotator = { AITemp->GetActorRotation().Pitch,DifRotation.Yaw,AITemp->GetActorRotation().Roll };
			AITemp->SetActorRotation(newRotator);
			UAnimMontage* CHM = Cast<UAnimMontage>(BB->GetValueAsObject("CurrentHittedMontage"));
			if (CHM)
			{
				AITemp->PlayHittedMontage(CHM);
				return EBTNodeResult::InProgress;
			}
			else
				return EBTNodeResult::Failed;
		}
		else
			return EBTNodeResult::Failed;
	}
		return EBTNodeResult::Failed;
}

void UBTT_AIBeHitted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ANormalEnemyController* AIController = Cast<ANormalEnemyController>(OwnerComp.GetAIOwner());
	UWorld* world = GetWorld();
	if (AIController)
	{
		ANormalEnemyCharacter* AITemp = Cast<ANormalEnemyCharacter>(AIController->GetPawn());
		if (AITemp)
			if(!AITemp->GetHittedValue() || AITemp->GetHitAgainBoolean())
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
