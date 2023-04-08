// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDCheckCloseEnough.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyController.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"

UBTDCheckCloseEnough::UBTDCheckCloseEnough()
{
}

bool UBTDCheckCloseEnough::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ANormalEnemyController* AIC = Cast<ANormalEnemyController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		FVector AILocation = Cast<ANormalEnemyCharacter>(AIC->GetPawn())->GetActorLocation();
		APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());//��ȡ�����
		if (player)
		{
			FVector PlayerLocation = player->GetActorLocation();
			//�������
			float Dis = (PlayerLocation - AILocation).Size();
			if (Dis < fDis)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
