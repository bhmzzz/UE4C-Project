// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashBodyEnd_AnimNotify.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

UFlashBodyEnd_AnimNotify::UFlashBodyEnd_AnimNotify()
{

}

void UFlashBodyEnd_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player)
		player->ClearFlashLocationEndHandler();
}
