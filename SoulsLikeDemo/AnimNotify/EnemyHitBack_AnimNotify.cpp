// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHitBack_AnimNotify.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"


UEnemyHitBack_AnimNotify::UEnemyHitBack_AnimNotify()
{
}

void UEnemyHitBack_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UWorld* world = GetWorld();
	Super::Notify(MeshComp, Animation);
}
