// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTAIMoveTo.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "NavigationSystem.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

UBTTAIMoveTo::UBTTAIMoveTo()
{
	NodeName = "AIMoveto";
	bNotifyTick = true;
	DistanceType = 0;
}

EBTNodeResult::Type UBTTAIMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANormalEnemyController* AIC = Cast<ANormalEnemyController>(OwnerComp.GetAIOwner());//通过行为树获取到AIController
	if (AIC)
	{
		UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetActor"));//获取到黑板中的值
		AActor* playerTemp = Cast<AActor>(TargetObject);
		DistanceType = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(MoveToSelector.SelectedKeyName);
		ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());//获得控制的AI
		if (AI && playerTemp)
		{
			UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			if (NavSys)
			{
				DebugNS::MyDebug(1.f, "FindNav");
				NavSys->SimpleMoveToActor(AIC, playerTemp);
				return EBTNodeResult::InProgress;
			}
			else return EBTNodeResult::Failed;
		}
		else
			return EBTNodeResult::Failed;
	}
	else
		return EBTNodeResult::Failed;
}

void UBTTAIMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ANormalEnemyController* AIC = Cast<ANormalEnemyController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		APawn* AI = AIC->GetPawn();
		FVector AILocation = Cast<ANormalEnemyCharacter>(AIC->GetPawn())->GetActorLocation();
		APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());//获取到玩家
		FVector PlayerLocation = player->GetActorLocation();
		if (player && AI)
		{
			switch (DistanceType)
			{
			case 0:
			{
				if (player->GetDisWithActor(AI) - 900.f < 0.01f)
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				break;
			}
			case 1:
			{
				if (player->GetDisWithActor(AI) - 450.f < 0.01f)
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				break;
			}
			case 2:
			{
				if (player->GetDisWithActor(AI) - 190.f < 0.01f)
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				break;
			}
			default:
				break;
			}
		}
	}
}
