// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameMode.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"


AMainPlayerController::AMainPlayerController()
{
}

bool AMainPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	//这里判断此时按键是否被包含在需要忽略的按键中，如果包含那么直接返回false则此按键此时不会生效
	if (IgnoredKeys.Contains(Key))
		return false;
	return Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
}

void AMainPlayerController::BeginPlay()
{
	//游戏开始时处于菜单状态，显示鼠标
	GameModeIns = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	HudIns = Cast<AMainHUD>(GetHUD());
	if (GameModeIns->GetCurrentGameMode() == EGAME_MODE::Type::EMenuUI)
		bShowMouseCursor = true;
	FInputModeUIOnly UIInput;
	UIInput.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(UIInput);
	/*//可自定义设置在不同状态的输入类型
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);*/
	//..
}

void AMainPlayerController::SetNormalInputMode()
{
	if (GameModeIns && HudIns)
	{
		GameModeIns->SetCurrentGameMode(EGAME_MODE::Type::EInGame);
		bShowMouseCursor = false;
		//关闭MenuWidget
		HudIns->RemoveMenuWidget();
		HudIns->AddItemWidgetToViewPort();

		FInputModeGameOnly GameInput;
		SetInputMode(GameInput);
	}
}

void AMainPlayerController::SetUIInputMode()
{
	GameModeIns->SetCurrentGameMode(EGAME_MODE::Type::EMenuUI);
	bShowMouseCursor = true;
	FInputModeUIOnly UIInput;
	SetInputMode(UIInput);
}

void AMainPlayerController::SwitchMagicItem()
{
	if (HudIns)
	{
		HudIns->SwitchMagicItem();
	}
}

void AMainPlayerController::SwitchMedcineItem()
{
	if (HudIns)
	{
		HudIns->SwitchMedcineItem();
	}
}

void AMainPlayerController::SwitchACKMode(FString& ACKMode)
{
	if (HudIns)
		HudIns->SwitchACKMode(ACKMode);
}

void AMainPlayerController::OpenFireBladeUI()
{
	if (GameModeIns && HudIns)
	{
		GameModeIns->SetCurrentGameMode(EGAME_MODE::Type::EGamePause);
		bShowMouseCursor = true;

		FInputModeUIOnly UIInput;
		SetInputMode(UIInput);

		HudIns->AddFireBladeWidgetToViewPort();
	}
}

void AMainPlayerController::ToggleInputKey(bool bIngore, FKey Key)
{
	if (bIngore)
		IgnoredKeys.AddUnique(Key);
	else
		IgnoredKeys.Remove(Key);
}

FString AMainPlayerController::GetCurrentItem()
{
	return HudIns->GetCurrentItem();
}

void AMainPlayerController::SetItemNum(uint8 num)
{
	if (HudIns)
		HudIns->SetItemNum(num);
}

