// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimInstance();

protected:
	UPROPERTY(BlueprintReadOnly)
		float PlayerSpeed;
	UPROPERTY(BlueprintReadOnly)
		float PlayerDirection;
	UPROPERTY(BlueprintReadOnly)
		float InputX;
	UPROPERTY(BlueprintReadOnly)
		float InputY;
public:
	UFUNCTION(BlueprintCallable)
	float UpdateSpeed();
	UFUNCTION(BlueprintCallable)
	float UpdateDirection();
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerInput();
	UFUNCTION(BlueprintCallable)
		bool CanAirACK();
};
