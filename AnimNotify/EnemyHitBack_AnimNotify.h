// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnemyHitBack_AnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UEnemyHitBack_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UEnemyHitBack_AnimNotify();

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
protected:
	FTimerHandle handler;
};
