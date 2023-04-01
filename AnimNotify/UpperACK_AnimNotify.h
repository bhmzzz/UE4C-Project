// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UpperACK_AnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UUpperACK_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UUpperACK_AnimNotify();
	void Notify(USkeletalMeshComponent* MenshComp, UAnimSequenceBase* AnimSeq) override;
};
