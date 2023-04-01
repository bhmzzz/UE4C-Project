// Fill out your copyright notice in the Description page of Project Settings.


#include "SFireBladeWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "MenuStyleHelper.h"
#include "MenuWidgetStyle.h"
#include "Widgets/Layout/SBackgroundBlur.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SFireBladeWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &MenuStyleHelper::Get().GetWidgetStyle<FMenuStyle>("MenuStyle");

	ChildSlot
	[
		// Populate the widget
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.f,0.f,1.f,1.f))
		.Offset(FMargin(0.f))
		[
			SNew(SBackgroundBlur)
			.BlurStrength(5.f)
		]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f))
		.Offset(FMargin(0.f,-33.f,400.f,600.f))
		.Alignment(FVector2D(0.5f,0.5f))
		[
			SNew(SBox)
			.RenderTransformPivot(FVector2D(0.5f,0.5f))
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.FillHeight(0.1f)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(&MenuStyle->MenuTopBrush)
				]
				+ SVerticalBox::Slot()
				.FillHeight(0.9f)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(&MenuStyle->BGBrush)
					.Padding(20.f)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.Padding(50.f,50.f,50.f,0.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("BladeFire_Text","Save_Game","保存游戏"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(50.f, 50.f, 50.f, 0.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SAssignNew(SkillButton,SButton)
							.OnClicked(this, &SFireBladeWidget::SkillBtn_OnCLicked)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("BladeFire_Text", "Skill", "技能"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(50.f, 50.f, 50.f, 0.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("BladeFire_Text", "DoublePlayer", "双人游戏"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
						+ SVerticalBox::Slot()
						.Padding(50.f, 100.f, 50.f, 0.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SAssignNew(CloseButton,SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.OnClicked(this,&SFireBladeWidget::CloseBtn_OnCLicked)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("BladeFire_Text", "close", "关闭"))
								.Font(MenuStyle->CommonFont)
								.Justification(ETextJustify::Center)
							]
						]
					]
				]
			]
		]
	];
	
}
FReply SFireBladeWidget::CloseBtn_OnCLicked()
{
	UWorld* world = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(world, 0)->GetHUD());
	if (HudIns)
		HudIns->RemoveBladeWidget();
	return FReply::Handled();
}
FReply SFireBladeWidget::SkillBtn_OnCLicked()
{
	UWorld* world = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(world, 0)->GetHUD());
	if (HudIns)
		HudIns->AddSkillWidgetToViewPort();
	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
