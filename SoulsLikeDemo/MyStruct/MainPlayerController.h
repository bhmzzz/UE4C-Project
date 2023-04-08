// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"
/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPlayerController();

	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;

protected:
	virtual void BeginPlay() override;

	class AMyGameMode* GameModeIns;
	class AMainHUD* HudIns;

	//需要临时禁止的按键
	TArray<FKey> IgnoredKeys;

public:
	void SetNormalInputMode();
	void SetUIInputMode();

	void SwitchMagicItem();
	void SwitchMedcineItem();
	void SwitchACKMode(FString& ACKMode);

	void OpenFireBladeUI();

	//添加或移除需要忽略的按键
	void ToggleInputKey(bool bIngore, FKey Key);
	FString GetCurrentItem();
	void SetItemNum(uint8 num);
};
