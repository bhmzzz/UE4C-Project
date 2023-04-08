// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "SlateBasics.h"
#include "SoulsLikeDemo/Widget/SEnemyWidget.h"
#include "SoulsLikeDemo/Widget/SMenuWidget.h"
#include "SoulsLikeDemo/Widget/SItemWidget.h"
#include "SoulsLikeDemo/Widget/SFireBladeWidget.h"
#include "SoulsLikeDemo/Widget/SSkillWidget.h"
#include "SoulsLikeDemo/Widget/SWinWidget.h"
#include "SoulsLikeDemo/Widget/SGameOverWidget.h"
#include "SoulsLikeDemo/MyStruct/MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/MyStruct/MainPlayerController.h"
#include "Components/VerticalBox.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/FileMediaSource.h"
#include "Materials/Material.h"
#include "SoulsLikeDemo/SaveGame/MySaveGame.h"


AMainHUD::AMainHUD()
{
	if (GEngine && GEngine->GameViewport)
	{
		SAssignNew(MenuWidget, SMenuWidget);
		SAssignNew(EnemyWidget, SEnemyWidget);
		SAssignNew(ItemWidget, SItemWidget);
		SAssignNew(FireBladeWidget, SFireBladeWidget);
		SAssignNew(SkillWidget, SSkillWidget);
		SAssignNew(WinWidget, SWinWidget);
		SAssignNew(GameOverWidget, SGameOverWidget);
	}
}

void AMainHUD::BeginPlay()
{
	GameModeIns = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameModeIns->SetCurrentGameMode(EGAME_MODE::Type::EMenuUI);
	//在游戏开始时显示主菜单
	AddMenuWidgetToViewPort();

}

TSharedPtr<class SMenuWidget> AMainHUD::GetMenuWidget()
{
	return MenuWidget;
}

TSharedPtr<class SEnemyWidget> AMainHUD::GetEnemyWidget()
{
	return EnemyWidget;
}

void AMainHUD::AddEnemyWidgetToViewPort()
{
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(EnemyWidget.ToSharedRef()));
}

void AMainHUD::AddMenuWidgetToViewPort()
{
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
}

void AMainHUD::AddItemWidgetToViewPort()
{
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(ItemWidget.ToSharedRef()));
}

void AMainHUD::AddFireBladeWidgetToViewPort()
{
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(FireBladeWidget.ToSharedRef()));
	if (ItemWidget)
		ItemWidget->ChangeItemNumber(2);
}

void AMainHUD::AddSkillWidgetToViewPort()
{
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(SkillWidget.ToSharedRef()));
}

void AMainHUD::AddWinWidgetToViewPort()
{
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(WinWidget.ToSharedRef()));
}

void AMainHUD::AddGameOverWidgetToViewPort()
{
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameOverWidget.ToSharedRef()));
}

void AMainHUD::RemoveMenuWidget()
{
	GEngine->GameViewport->RemoveAllViewportWidgets();
}

void AMainHUD::RemoveBladeWidget()
{
	if (FireBladeWidget.IsValid() && GEngine && GEngine->GameViewport)
	{
		AMainPlayerController* pc = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (pc)
			pc->SetNormalInputMode();
		GEngine->GameViewport->RemoveViewportWidgetContent(FireBladeWidget.ToSharedRef());
	}
}

void AMainHUD::RemoveSkillWidget()
{
	if (SkillWidget.IsValid() && GEngine && GEngine->GameViewport)
	{
		AMainPlayerController* pc = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (pc)
			pc->SetNormalInputMode();
		GEngine->GameViewport->RemoveViewportWidgetContent(SkillWidget.ToSharedRef());
	}
}

void AMainHUD::SwitchMagicItem()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player && ItemWidget.IsValid())
		ItemWidget->SwitchMagicItem(player->GetItemNum());
}

void AMainHUD::SwitchMedcineItem()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player && ItemWidget.IsValid())
		ItemWidget->SwitchMedcineItem(player->GetItemNum());
}

void AMainHUD::SwitchACKMode(FString& ACKMode)
{
	if (ItemWidget.IsValid())
		ItemWidget->SwitchACKMode(ACKMode);
}

TMap<ESkillRadioType, UMaterial*> AMainHUD::GetSkillAnim()
{
	return SkillAnim;
}

TMap<ESkillRadioType, UMediaPlayer*> AMainHUD::GetMedias()
{
	return SkillMedias;
}

TMap<ESkillRadioType, UFileMediaSource*> AMainHUD::GetAnimSources()
{
	return AnimSources;
}

FString AMainHUD::GetCurrentItem()
{
	return ItemWidget->GetCurrentItem();
}

void AMainHUD::SetItemNum(uint8 num)
{
	if (ItemWidget)
		ItemWidget->ChangeItemNumber(num);
}

void AMainHUD::SaveMyGame()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player)
		player->SaveMyGame();
}

void AMainHUD::ResetGameUI()
{
	GEngine->GameViewport->RemoveAllViewportWidgets();
	AddItemWidgetToViewPort();
}
