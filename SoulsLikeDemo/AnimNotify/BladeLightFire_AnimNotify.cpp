// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeLightFire_AnimNotify.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"

void UBladeLightFire_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(MeshComp->GetOwner());
	if (AI)
	{
		AI->SpawnBladeLight();
	}

}
