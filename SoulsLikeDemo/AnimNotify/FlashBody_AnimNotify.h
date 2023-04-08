// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FlashBody_AnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UFlashBody_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UFlashBody_AnimNotify();

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
