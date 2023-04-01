// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlayHittedCueAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UPlayHittedCueAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPlayHittedCueAnimNotify();
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* AnimSeq) override;

};
