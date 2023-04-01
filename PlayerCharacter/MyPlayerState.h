// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlayerStateMode : uint8
{
	ENORMAL,		//ÆÕÍ¨Î´×°±¸ÎäÆ÷×´Ì¬
	EEQUIP,			//×°±¸ÎäÆ÷×´Ì¬
	EACK,			//¹¥»÷
	ELOCK,          //Ëø¶¨
	EJUMP			//ÌøÔ¾
};

UCLASS()
class SOULSLIKEDEMO_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMyPlayerState();
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		class APlayerCharacter* player;
public:
	UFUNCTION()
		void Set_NormalMode(EPlayerStateMode& PlayerMode);
	UFUNCTION()
		void Set_EquipMode(EPlayerStateMode& PlayerMode);
	UFUNCTION()
		void Set_ACKMode(EPlayerStateMode& PlayerMode);
	UFUNCTION()
		void Set_JumpMode(EPlayerStateMode& PlayerMode);
	UFUNCTION()
		void Set_LockMode(EPlayerStateMode& PlayerMode);
	
};
