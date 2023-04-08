// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BladeLightFire_AnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UBladeLightFire_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
