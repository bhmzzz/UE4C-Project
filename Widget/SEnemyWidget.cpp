// Fill out your copyright notice in the Description page of Project Settings.


#include "SEnemyWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEnemyWidget::Construct(const FArguments& InArgs)
{
	ProgressbarAnim = FCurveSequence(0.f,1.5f);
	ChildSlot
		[
			SNew(SConstraintCanvas)
			+ SConstraintCanvas::Slot()
			.Anchors(TAttribute<FAnchors>(FAnchors(0.f, 1.f, 1.f, 1.f)))
			.Offset(FMargin(200.f, -80.f, 200.f, 30.f))
				[
					SAssignNew(Enemy_HPB,SProgressBar)
					.Percent(0.f)
					.FillColorAndOpacity(FLinearColor(1.f,0.1f,0.f,1.f))
				]
	       + SConstraintCanvas::Slot()
		   .Anchors(FAnchors(0.f,1.f,1.f,1.f))
		   .Offset(FMargin(700.f,-136.f,700.f,50.f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(0.4)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					.VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("BOOS:"))
					    .Font(FCoreStyle::GetDefaultFontStyle("Roboto", 24))
						.Justification(ETextJustify::Center)
					]
			+ SHorizontalBox::Slot()
		   .FillWidth(0.6f)
		   .HAlign(EHorizontalAlignment::HAlign_Fill)
		   .VAlign(EVerticalAlignment::VAlign_Center)
					[
					SNew(STextBlock)
					.Text(FText::FromString("No.1"))
					.Font(FCoreStyle::GetDefaultFontStyle("Roboto", 24))
					.Justification(ETextJustify::Center)
					]
				]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f,1.f,0.5f,1.f))
		.Offset(FMargin(-757,-161, 200,74))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(0.5f)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
			    .HAlign(EHorizontalAlignment::HAlign_Center)
			    .VAlign(EVerticalAlignment::VAlign_Center)
			    [
					SNew(STextBlock)
					.Text(NSLOCTEXT("Enemy_TEXT","JUEQI","绝气"))
					.Font(FCoreStyle::GetDefaultFontStyle("Roboto", 24))
			        .Justification(ETextJustify::Center)
				]
				+ SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("Enemy_TEXT", "BATI", "霸体"))
					.Font(FCoreStyle::GetDefaultFontStyle("Roboto", 24))
					.Justification(ETextJustify::Center)
				]
			]
			+ SHorizontalBox::Slot()
				.FillWidth(0.5f)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Fill)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("Enemy_TEXT", "LEIJI", "雷击"))
					.Font(FCoreStyle::GetDefaultFontStyle("Roboto", 24))
					.Justification(ETextJustify::Center)
				]
			+ SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("Enemy_TEXT", "RANSHAO", "燃烧"))
					.Font(FCoreStyle::GetDefaultFontStyle("Roboto", 24))
					.Justification(ETextJustify::Center)
				]
				]
		]
	];
	
}
void SEnemyWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!bProgressBarFull)
	{
		if (!ProgressbarAnim.IsAtEnd())
			Enemy_HPB->SetPercent(ProgressbarAnim.GetLerp());
		else
		{
			bProgressBarFull = true;
			Enemy_HPB->SetPercent(ProgressbarAnim.GetLerp());
		}
	}
}
void SEnemyWidget::ProgressBarChange()
{
	if (!ProgressbarAnim.IsPlaying())
		ProgressbarAnim.Play(this->AsShared());
}
bool SEnemyWidget::GetProgressBarFullBoolean()
{
	return bProgressBarFull;
}
void SEnemyWidget::ChangeAI_OptionBar(TMap<AI_Option::Type, float> AIOptionValue)
{
	//设置AI的HP血条变动
	if (Enemy_HPB.IsValid())
	{
		Enemy_HPB->SetPercent(FMath::Clamp<float>(AIOptionValue[AI_Option::Type::EHP]/100.f,0.f,1.f));
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
