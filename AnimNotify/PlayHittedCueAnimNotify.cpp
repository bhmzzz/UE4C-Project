// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayHittedCueAnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimSequenceBase.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

UPlayHittedCueAnimNotify::UPlayHittedCueAnimNotify()
{

}

void UPlayHittedCueAnimNotify::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (player && !player->GetPlayACKSoundBoolean())
		player->PlaySwingSound();
}

