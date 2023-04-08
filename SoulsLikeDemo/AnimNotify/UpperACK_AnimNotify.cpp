// Fill out your copyright notice in the Description page of Project Settings.


#include "UpperACK_AnimNotify.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"

UUpperACK_AnimNotify::UUpperACK_AnimNotify()
{
}

void UUpperACK_AnimNotify::Notify(USkeletalMeshComponent* MenshComp, UAnimSequenceBase* AnimSeq)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(MenshComp->GetOwner());
	if (Player)
	{
		Player->UpperPlayer();
		bool CanUpperAI = Player->CheckUpperAI();
		if (CanUpperAI)
		{
			TArray<AActor*> Target = Player->GetUpperTarget();
			if(Target.Num() != 0)
				for (const auto& Value : Target)
				{
					ANormalEnemyCharacter* AITarget = Cast<ANormalEnemyCharacter>(Value);
					if (AITarget)
						AITarget->UpperAI();
				}
		}
	}
}
