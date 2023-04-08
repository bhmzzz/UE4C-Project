// Fill out your copyright notice in the Description page of Project Settings.


#include "SMenuWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "MenuWidgetStyle.h"
#include "MenuStyleHelper.h"
#include "Styling/SlateTypes.h"
#include "SoulsLikeDemo/MyStruct/MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMenuWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器中的style,参数为蓝图中该style的FName
	MenuStyle = &MenuStyleHelper::Get().GetWidgetStyle<FMenuStyle>("MenuStyle");

	//不能这样写，这样会改变源码
	/*NewButtonStyle = const_cast<FButtonStyle*>(& FCoreStyle::Get().GetWidgetStyle< FButtonStyle >("Button"));
	NewButtonStyle->SetNormal(MenuStyle->ButtonBrush);*/
	
	ChildSlot
	[
		// Populate the widget
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f))
		.Offset(FMargin(350.f,-260.f,930.f,250.f))
		.Alignment(FVector2D(0.5f,1.f))
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(NSLOCTEXT("Menu_TEXT","GameTitle","C++ DEMO"))
				.ColorAndOpacity(FLinearColor(0.16f,0.05f,1.f,1.f))
				.Font(MenuStyle->GameTitleFont/*FCoreStyle::GetDefaultFontStyle("Roboto",100)*/)
				.ShadowOffset(FVector2D(1.f,1.f))
				.Justification(ETextJustify::Center)
			]
		]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.f))
		.Offset(FMargin(95.f,60.f,450.f,530.f))
		.Alignment(FVector2D(0.f))
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(&MenuStyle->BGBrush)
				.Padding(FMargin(4.f,2.f,4.f,2.f))
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Top)
					[
						SNew(SBox)
						.HeightOverride(30.f)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SBorder)
							.BorderImage(&MenuStyle->MenuTopBrush)
							.Padding(FMargin(4.f,2.f,4.f,2.f))
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(NSLOCTEXT("Menu_TEXT","MenuTitle","MENU"))
								.ColorAndOpacity(FLinearColor(1.f, 0.7f, 0.f, 1.f))
								.Font(MenuStyle->MenuTitleFont)
								.Justification(ETextJustify::Center)
							]
						]
					]
					+ SOverlay::Slot()
					.Padding(FMargin(20.f,40.f,20.f,10.f))
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SBorder)
						.BorderImage(&MenuStyle->MenuBGBrush)
						.Padding(FMargin(0.f,20.f,4.f,2.f))
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Top)
						[
							SNew(SBox)
							.WidthOverride(150.f)
							.HeightOverride(300.f)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.Padding(FMargin(0.f,20.f,0.f,0.f))
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								[
									SNew(SButton)
									.ContentPadding(FMargin(4.f,2.f,4.f,2.f))
									.OnClicked(this,&SMenuWidget::GameStart_OnClicked)
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
										.Text(NSLOCTEXT("Menu_TEXT","GameStart","开始游戏"))
										.ColorAndOpacity(FLinearColor(0.93f,1.f,0.f,1.f))
										.Font(MenuStyle->CommonFont)
										.Justification(ETextJustify::Center)
									]
								]
								+ SVerticalBox::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SSpacer)
									.Size(FVector2D(1.f,20.f))
								]
								+ SVerticalBox::Slot()
									.HAlign(HAlign_Fill)
									.VAlign(VAlign_Center)
									[
									SNew(SButton)
									.OnClicked(this, &SMenuWidget::GameEnd_OnClicked)
									.ContentPadding(FMargin(4.f, 2.f, 4.f, 2.f))
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
										.Text(NSLOCTEXT("Menu_TEXT", "GameOver", "结束游戏"))
										.ColorAndOpacity(FLinearColor(0.93f, 1.f, 0.f, 1.f))
										.Font(MenuStyle->CommonFont)
										.Justification(ETextJustify::Center)
									]
								]
								+ SVerticalBox::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SSpacer)
									.Size(FVector2D(1.f, 20.f))
								]
							/*	+ SVerticalBox::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								[
									SNew(SButton)
									.ContentPadding(FMargin(4.f, 2.f, 4.f, 2.f))
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
										.Text(NSLOCTEXT("Menu_TEXT", "Other", "功能概要"))
										.ColorAndOpacity(FLinearColor(0.93f, 1.f, 0.f, 1.f))
										.Font(MenuStyle->CommonFont)
										.Justification(ETextJustify::Center)
									]
								]*/
							]
						]
					]
				]
			]
		]
	];
}
FReply SMenuWidget::GameStart_OnClicked()
{
	//这样获取GameMode的话只有单独开一个线程才可以，编辑器下运行会报错。
	UWorld* World = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (player)
	{
		player->PlaySitUp();
	}
	return FReply::Handled();
}
FReply SMenuWidget::GameEnd_OnClicked()
{
	UWorld* world = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	UKismetSystemLibrary::ExecuteConsoleCommand(world, "Exit");
	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
