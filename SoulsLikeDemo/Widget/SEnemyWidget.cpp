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
			.Anchors(TAttribute<FAnchors>(FAnchors(0.f, 1.f, 1.f, 1.f)))
			.Offset(FMargin(200.f, -50.f, 200.f, 15.f))
			[
				SAssignNew(Enemy_AMB, SProgressBar)
				.Percent(0.f)
				.FillColorAndOpacity(FLinearColor(1.f, 0.95f, 0.f, 1.f))
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
					SAssignNew(JQ_Text,STextBlock)
					//.Text(NSLOCTEXT("Enemy_TEXT","JUEQI","绝气"))
					.Text(NSLOCTEXT("Enemy_TEXT", "None", "无"))
					.Font(FCoreStyle::GetDefaultFontStyle("Roboto", 24))
			        .Justification(ETextJustify::Center)
				]
				+ SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SAssignNew(BT_Text, STextBlock)
					.Text(NSLOCTEXT("Enemy_TEXT", "BATI", "霸体"))
					.ColorAndOpacity(FLinearColor(0.f,1.f,0.f,1.f))
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
					SAssignNew(LJ_Text, STextBlock)
					//.Text(NSLOCTEXT("Enemy_TEXT", "LEIJI", "雷击"))
					.Text(NSLOCTEXT("Enemy_TEXT", "None", "无"))
					.Font(FCoreStyle::GetDefaultFontStyle("Roboto", 24))
					.Justification(ETextJustify::Center)
				]
			+ SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SAssignNew(RS_Text, STextBlock)
					//.Text(NSLOCTEXT("Enemy_TEXT", "RANSHAO", "燃烧"))
					.Text(NSLOCTEXT("Enemy_TEXT", "None", "无"))
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
		{
			Enemy_HPB->SetPercent(ProgressbarAnim.GetLerp());
			Enemy_AMB->SetPercent(ProgressbarAnim.GetLerp());
		}
		else
		{
			bProgressBarFull = true;
			Enemy_HPB->SetPercent(ProgressbarAnim.GetLerp());
			Enemy_AMB->SetPercent(ProgressbarAnim.GetLerp());
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
	if (Enemy_HPB.IsValid() && Enemy_AMB.IsValid())
	{
		if(AIOptionValue.Contains(AI_Option::Type::EHP))
			Enemy_HPB->SetPercent(FMath::Clamp<float>(AIOptionValue[AI_Option::Type::EHP]/100.f,0.f,1.f));
		if (AIOptionValue.Contains(AI_Option::Type::EAM))
			Enemy_AMB->SetPercent(FMath::Clamp<float>(AIOptionValue[AI_Option::Type::EAM] / 100.f, 0.f, 1.f));
	}
}
void SEnemyWidget::ChangeAI_State(EBossStateType stateArr)
{
	switch (stateArr)
	{
	case EBossStateType::BT:
		if (BT_Text.IsValid() && JQ_Text.IsValid())
		{
			BT_Text->SetText(NSLOCTEXT("Enemy_TEXT", "BATI", "霸体"));
			BT_Text->SetColorAndOpacity(FLinearColor(0.f, 1.f, 0.f, 1.f));
			JQ_Text->SetText(NSLOCTEXT("Enemy_TEXT", "None", "无"));
			JQ_Text->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		break;
	case EBossStateType::QJ:
		if (BT_Text.IsValid() && JQ_Text.IsValid())
		{
			BT_Text->SetText(NSLOCTEXT("Enemy_TEXT", "None", "无"));
			BT_Text->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			JQ_Text->SetText(NSLOCTEXT("Enemy_TEXT", "JUEQI", "气绝"));
			JQ_Text->SetColorAndOpacity(FLinearColor(1.f, 0.f, 1.f, 1.f));
		}
		break;
	case EBossStateType::LJ:
		if (LJ_Text.IsValid())
		{
			LJ_Text->SetText(NSLOCTEXT("Enemy_TEXT", "LEIJI", "雷击"));
			LJ_Text->SetColorAndOpacity(FLinearColor(1.f, 1.f, 0.f, 1.f));
		}
		break;
	case EBossStateType::RS:
		if (RS_Text.IsValid())
		{
			RS_Text->SetText(NSLOCTEXT("Enemy_TEXT", "RANSHAO", "燃烧"));
			RS_Text->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		break;
	default:
		DebugNS::MyDebug(1.f, "Error Boss State!");
		break;
	}
}
void SEnemyWidget::SetDebuffNone(EBossStateType Debuff)
{
	switch (Debuff)
	{
	case EBossStateType::LJ:
		if (LJ_Text)
		{
			LJ_Text->SetText(NSLOCTEXT("Enemy_TEXT", "None", "无"));
			LJ_Text->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		break;
	case EBossStateType::RS:
		if (RS_Text)
		{
			RS_Text->SetText(NSLOCTEXT("Enemy_TEXT", "None", "无"));
			RS_Text->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		break;
	default:
		DebugNS::MyDebug(1.f, "Error Debuff State!");
		break;
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
