// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FlashBodyEnd_AnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UFlashBodyEnd_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UFlashBodyEnd_AnimNotify();

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
