// Fill out your copyright notice in the Description page of Project Settings.


#include "SSkillWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "MenuStyleHelper.h"
#include "MenuWidgetStyle.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/MyStruct/MainPlayerController.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"
#include "Materials/Material.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/FileMediaSource.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSkillWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &MenuStyleHelper::Get().GetWidgetStyle<FMenuStyle>("MenuStyle");

	ChildSlot
	[
		// Populate the widget
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f))
		.Offset(FMargin(0.f,0.f,1000.f,800.f))
		.Alignment(FVector2D(0.5f,0.5f))
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(&MenuStyle->BGBrush)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.Padding(0.f,0.f,700.f,0.f)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.Padding(40.f,0.f,40.f,15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this,&SSkillWidget::PlayUpAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text","UpACK","上段攻击"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f,0.f,40.f,15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayMidAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "MidACK", "中段攻击"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlaySlideAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "Slide", "垫步"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayFlashAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "Flash", "瞬身"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayCloseFlashAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "CloseFlash", "极限闪避"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayAirACKAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "AirACK", "空中连击"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayCYZAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "CYZ", "次元斩"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayJuHeAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "JuHe", "居合"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayDownACKAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "DownACK", "落地斩"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayUpperAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "UpperACK", "上挑"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayOpenFireAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "OpenFire", "R! 倾泻子弹"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlayMissileAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "MissileFire", "R! 集束飞弹"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(40.f, 0.f, 40.f, 15.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.OnClicked(this, &SSkillWidget::PlaySkillPointsAnim)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Skill_Text", "SkillPoints", "技能点数"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
					]
					+ SOverlay::Slot()
					.Padding(300.f,0.f,0.f,0.f)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.FillHeight(0.6)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SBox)
							.Padding(20.f)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							[
								SAssignNew(AnimImage,SImage)
							]
						]
						+ SVerticalBox::Slot()
						.FillHeight(0.4)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SBox)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.FillHeight(0.8)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SBox)
									.HAlign(HAlign_Fill)
									.VAlign(VAlign_Fill)
									[
										SAssignNew(SkillIntroduce,STextBlock)
										.Font(MenuStyle->CommonFont)
										.AutoWrapText(true)
									]
								]
								+ SVerticalBox::Slot()
								.FillHeight(0.2)
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								[
									SAssignNew(CloseButton,SButton)
									.OnClicked(this,&SSkillWidget::CloseSkillWidget)
									.HAlign(HAlign_Fill)
									.VAlign(VAlign_Fill)
									[
										SNew(STextBlock)
										.Text(NSLOCTEXT("Skill_Text","Close","关闭"))
										.Font(MenuStyle->CommonFont)
									]
								]
							]
						]
					]
				]
			]
		]
	];
}
FReply SSkillWidget::CloseSkillWidget()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns)
	{
		if (CurrentMedia && CurrentMedia->IsPlaying())
			CurrentMedia->Pause();
		HudIns->RemoveSkillWidget();
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayUpAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "UpACKI", "     通过单击鼠标右键切换攻击模式，当攻击模式处于上段时，见玩家界面右下角为“上段”，在装备武器的情况下点击鼠标左键可进行上段攻击，上段攻击体力值消耗较大，但是伤害较高。"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::UpACK]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::UpACK], HudIns->GetAnimSources()[ESkillRadioType::UpACK]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayMidAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "MidACKI", "     通过单击鼠标右键切换攻击模式，当攻击模式处于中段时，见玩家界面右下角为“中段”，在装备武器的情况下点击鼠标左键可进行中段攻击，中段攻击体力值消耗较小，伤害较低，但是可以打出多段攻击。"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::MidACK]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::MidACK], HudIns->GetAnimSources()[ESkillRadioType::MidACK]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlaySlideAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "SlideI", "     通过鼠标中键锁定敌人，在锁定状态下时，可按下“Alt”进行垫步躲避敌人的攻击。"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::Slide]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::Slide], HudIns->GetAnimSources()[ESkillRadioType::Slide]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayFlashAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "FlashI", "     通过鼠标中键锁定敌人，在锁定状态下时，可按下空格键与‘W’或者‘S’键进行前向瞬身并留下残影或者后向翻滚，技巧：在空中发动前向瞬身可直接瞬移至敌人前方。（瞬身会消耗技能点数）"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::Flash]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::Flash], HudIns->GetAnimSources()[ESkillRadioType::Flash]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayCloseFlashAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "CloseFlashI", "     通过鼠标中键锁定敌人，在锁定状态下时，当敌人进攻时的近身位置处可按下“Alt”发动垫步躲避敌人的攻击，如果时机恰好可以触发完美闪避，标志为在完美闪避位置处留下残影，并且此时敌人进入动作缓慢状态，此时按下鼠标左键可发动追击，直接吸附至敌人身前，但是这段时间内敌人无法被打出硬直。（完美闪避会消耗技能点数）"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::CloseFlash]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::CloseFlash], HudIns->GetAnimSources()[ESkillRadioType::CloseFlash]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayAirACKAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "AirACKI", "     当玩家处于空中时，此时按下鼠标左键将进行空中连击。"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::AirACK]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::AirACK], HudIns->GetAnimSources()[ESkillRadioType::AirACK]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayCYZAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "CYZI", "     通过鼠标中键锁定敌人，在锁定状态下时，同时按下“LShift”与‘W’与“鼠标左键”，当鼠标左键按下时间不满2s时将触发次元斩，技巧：在空中时也可触发次元斩。（次元斩会消耗技能点数）"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::CYZ]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::CYZ], HudIns->GetAnimSources()[ESkillRadioType::CYZ]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayJuHeAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "JuHeI", "     通过鼠标中键锁定敌人，在锁定状态下时，同时按下“LShift”与‘W’与“鼠标左键”，当鼠标左键按下时间大于2s时将触发居合斩。（居合斩会消耗技能点数）"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::Juhe]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::Juhe], HudIns->GetAnimSources()[ESkillRadioType::Juhe]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayDownACKAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "DownACKI", "     当玩家处于空中时，同时按下“LShift”与‘S’与“鼠标左键”，将会触发落地斩。"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::DownACK]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::DownACK], HudIns->GetAnimSources()[ESkillRadioType::DownACK]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayUpperAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "UpperACKI", "     通过鼠标中键锁定敌人，在锁定状态下时，同时按下“LShift”与‘S’与“鼠标左键”，将会触发挑飞，如果敌人在判定范围内将会被一同挑起到空中。"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::UpperACK]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::UpperACK], HudIns->GetAnimSources()[ESkillRadioType::UpperACK]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayOpenFireAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "R_FireI", "     伴随机器人R，会通过收到玩家按下“Ctrl”键的指令发动子弹速射。（会持续消耗体力值）"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::R_Fire]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::R_Fire], HudIns->GetAnimSources()[ESkillRadioType::R_Fire]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlayMissileAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "R_MissileI", "     伴随机器人R，会通过收到玩家按下‘C’键的指令发射集束飞弹。（集束飞弹会消耗技能点数）"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::R_Missile]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::R_Missile], HudIns->GetAnimSources()[ESkillRadioType::R_Missile]);
	}
	return FReply::Handled();
}
FReply SSkillWidget::PlaySkillPointsAnim()
{
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD());
	if (HudIns->GetMedias().Num() != 0 && HudIns->GetAnimSources().Num() != 0)
	{
		if (SkillIntroduce)
			SkillIntroduce->SetText(NSLOCTEXT("Skill_Text", "SkillPointsI", "     技能点数会在玩家发动特殊攻击时被消耗，当点数为空时将无法发动技能，但随着时间的流逝技能点数会持续恢复。"));
		AnimImage->SetImage(&MenuStyle->SkillBrushs[ESkillRadioType::SkillPoints]);
		PlaySkillAnimRadio(HudIns->GetMedias()[ESkillRadioType::SkillPoints], HudIns->GetAnimSources()[ESkillRadioType::SkillPoints]);
	}
	return FReply::Handled();
}

void SSkillWidget::PlaySkillAnimRadio(UMediaPlayer* Media, UFileMediaSource* Source)
{
	if (CurrentMedia && CurrentMedia->IsPlaying())
		CurrentMedia->Pause();
	CurrentMedia = Media;
	if (Media && Source)
		Media->OpenSource(Source);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
