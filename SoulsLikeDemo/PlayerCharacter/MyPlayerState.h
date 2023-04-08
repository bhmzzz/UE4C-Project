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
	ENORMAL,		//普通未装备武器状态
	EEQUIP,			//装备武器状态
	EACK,			//攻击
	ELOCK,          //锁定
	EJUMP			//跳跃
};

UCLASS()
class SOULSLIKEDEMO_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMyPlayerState();
protected:
	virtual void BeginPlay() override;

		class APlayerCharacter* player;
		class AMainPlayerController* PCIns;
		class AMainHUD* HudIns;
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
	UFUNCTION()
		void GetGameSaveData(class UMySaveGame* Save);
	
};
