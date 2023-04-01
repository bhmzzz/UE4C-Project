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
