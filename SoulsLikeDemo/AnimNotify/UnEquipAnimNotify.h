// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UnEquipAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UUnEquipAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UUnEquipAnimNotify();
	void Notify(USkeletalMeshComponent* MenshComp, UAnimSequenceBase* AnimSeq) override;
};
