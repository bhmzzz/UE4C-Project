// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AIUpper.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "AIController.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "kismet/KismetMathLibrary.h"
#include "kismet/GameplayStatics.h"

UBTT_AIUpper::UBTT_AIUpper()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_AIUpper::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	if (AI)
	{
		//设置AI空中受击Launch
		AI->BeAirCombEffect();
		//设置AI面向玩家
		FRotator DifRotation = UKismetMathLibrary::FindLookAtRotation(AI->GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
		FRotator newRotator = { AI->GetActorRotation().Pitch,DifRotation.Yaw,AI->GetActorRotation().Roll };
		AI->SetActorRotation(newRotator);

		//设置AI相对于玩家位置的吸附
		AI->AirHittedReSetLocation(80.f);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTT_AIUpper::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
