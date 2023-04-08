// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckDistance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "AIController.h"
#include "Kismet/GamePlayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

UBTS_CheckDistance::UBTS_CheckDistance()
{
	//bCreateNodeInstance = true;	//开启任务模块可被实例化，节点化。
}

void UBTS_CheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIC = OwnerComp.GetAIOwner();
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(AIC->GetPawn());
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AI && player && BBComp)
	{
		float AIToPlayerDis = player->GetDisWithActor(AI);
		if (AIToPlayerDis > 1000.f)
		{
			AI->GetCurrDistanceType() = EDistanceType::EFar;
			BBComp->SetValueAsEnum(DistanceSelector.SelectedKeyName, 0);    
		}
		else if (AIToPlayerDis > 500.f && AIToPlayerDis <= 1000.f)
		{
			AI->GetCurrDistanceType() = EDistanceType::EMid;
			BBComp->SetValueAsEnum(DistanceSelector.SelectedKeyName, 1);
		}
		else if (AIToPlayerDis > 250.f && AIToPlayerDis <= 500.f)
		{
			AI->GetCurrDistanceType() = EDistanceType::EMTN;
			BBComp->SetValueAsEnum(DistanceSelector.SelectedKeyName, 2);
		}
		else if (AIToPlayerDis > 200 && AIToPlayerDis <= 250.f)
		{
			AI->GetCurrDistanceType() = EDistanceType::ENear;
			BBComp->SetValueAsEnum(DistanceSelector.SelectedKeyName, 3);
		}
		else if (AIToPlayerDis <= 200)
		{
			AI->GetCurrDistanceType() = EDistanceType::EClose;
			BBComp->SetValueAsEnum(DistanceSelector.SelectedKeyName, 4);
		}
	}

}
