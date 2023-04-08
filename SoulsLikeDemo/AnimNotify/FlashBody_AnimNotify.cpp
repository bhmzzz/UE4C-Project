// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashBody_AnimNotify.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

UFlashBody_AnimNotify::UFlashBody_AnimNotify()
{
}

void UFlashBody_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player)
	{
		//如果是空瞬，那么直接到AI面前
		if (player->PlayerIsInAir())
			player->AirFlash();
		else player->SetPlayerMoveLoc();
	}
}
