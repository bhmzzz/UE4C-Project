// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UEnemyAnimInstance();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;
public:
	UFUNCTION(BlueprintCallable)
		float UpdateDate();
};
