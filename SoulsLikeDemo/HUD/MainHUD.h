// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
//Widget创建的类前面都有一个S，这里做前向声明
class SMenuWidget;

UENUM(BlueprintType)
enum class ESkillRadioType : uint8
{
	UpACK,
	MidACK,
	Slide,
	Flash,
	CloseFlash,
	AirACK,
	CYZ,
	Juhe,
	DownACK,
	UpperACK,
	R_Fire,
	R_Missile,
	SkillPoints
};

UCLASS()
class SOULSLIKEDEMO_API AMainHUD : public AHUD
{
	GENERATED_BODY()
public:
		AMainHUD();

protected:
	virtual void BeginPlay() override;

	//以智能指针的形式进行传递widget控件。
	TSharedPtr<class SMenuWidget> MenuWidget;

	TSharedPtr<class SEnemyWidget> EnemyWidget;

	TSharedPtr<class SItemWidget> ItemWidget;

	TSharedPtr<class SFireBladeWidget> FireBladeWidget;

	TSharedPtr<class SSkillWidget> SkillWidget;

	TSharedPtr<class SWinWidget> WinWidget;

	TSharedPtr<class SGameOverWidget> GameOverWidget;

	class AMyGameMode* GameModeIns;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SkillMedias")
	TMap<ESkillRadioType, class UMediaPlayer*> SkillMedias;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillMedias")
	TMap<ESkillRadioType,class UMaterial*> SkillAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillMedias")
	TMap<ESkillRadioType, class UFileMediaSource*> AnimSources;

public:
	TSharedPtr<class SMenuWidget> GetMenuWidget();
	TSharedPtr<class SEnemyWidget> GetEnemyWidget();
	UFUNCTION()
		void AddEnemyWidgetToViewPort();
	UFUNCTION()
		void AddMenuWidgetToViewPort();
	UFUNCTION()
		void AddItemWidgetToViewPort();
	UFUNCTION()
		void AddFireBladeWidgetToViewPort();
	UFUNCTION()
		void AddSkillWidgetToViewPort();
	UFUNCTION()
		void AddWinWidgetToViewPort();
	UFUNCTION()
		void AddGameOverWidgetToViewPort();
	UFUNCTION()
		void RemoveMenuWidget();
	UFUNCTION()
		void RemoveBladeWidget();
	UFUNCTION()
		void RemoveSkillWidget();
	UFUNCTION()
		void SwitchMagicItem();
	UFUNCTION()
		void SwitchMedcineItem();
	UFUNCTION()
		void SwitchACKMode(FString& ACKMode);
	UFUNCTION()
		TMap<ESkillRadioType, UMaterial*> GetSkillAnim();
	UFUNCTION()
		TMap<ESkillRadioType, UMediaPlayer*> GetMedias();
	UFUNCTION()
		TMap<ESkillRadioType, UFileMediaSource*> GetAnimSources();
	FString GetCurrentItem();
	void SetItemNum(uint8 num);

	void SaveMyGame();

	void ResetGameUI();

};
