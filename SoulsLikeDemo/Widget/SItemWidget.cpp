// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "SoulsLikeDemo/Widget/MenuStyleHelper.h"
#include "SoulsLikeDemo/Widget/MenuWidgetStyle.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SItemWidget::Construct(const FArguments& InArgs)
{
	MenuStyle =  &MenuStyleHelper::Get().GetWidgetStyle<FMenuStyle>("MenuStyle");

	ChildSlot
	[
		// Populate the widget
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(1.f))
		.Offset(FMargin(0.f,0.f,150.f,150.f))
		.Alignment(FVector2D(1.f,1.f))
		[
			SNew(SBorder)
			.BorderImage(&MenuStyle->ItemBorderBrush)
			.Padding(20.f)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(MagicItemImage,SImage)
					.Image(&MenuStyle->ItemBrushs[0])
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.Padding(0.f,0.f,20.f,0.f)
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("Item_TEXT", "NumberTitle", "x"))
					.ColorAndOpacity(FLinearColor(0.f, 1.f, 1.f, 1.f))
					.Font(MenuStyle->ItemNumberFont)
					.Justification(ETextJustify::Center)
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				[
					SAssignNew(MagicItemNumber,STextBlock)
					.Text(NSLOCTEXT("Item_TEXT","Number","0"))
					.ColorAndOpacity(FLinearColor(0.f,1.f,1.f,1.f))
					.Font(MenuStyle->ItemNumberFont)
					.Justification(ETextJustify::Center)
				]
			]
		]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(1.f))
		.Offset(FMargin(0.f,-150.f,150.f,50.f))
		.Alignment(FVector2D(1.f,1.f))
		[
			SAssignNew(MagicItemName,STextBlock)
			.Text(NSLOCTEXT("Item_TEXT","FirePaper","招火符"))
			.ColorAndOpacity(FLinearColor(1.f,0.76f,0.f,1.f))
			.Font(MenuStyle->ItemNameFont)
			.Justification(ETextJustify::Center)
		]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(1.f))
		.Offset(FMargin(150.f, 160.f, 150.f, 75.f))
		.Alignment(FVector2D(2.f, 6.f))
		[
			SAssignNew(ACKModeText,STextBlock)
			.Text(NSLOCTEXT("Item_TEXT","UpMode","上段"))
			.ColorAndOpacity(FLinearColor(0.36f,0.05f,1.f,1.f))
			.Font(MenuStyle->ACKModeFont)
			.Justification(ETextJustify::Center)
		]
	];
	
}
void SItemWidget::SwitchMagicItem(TMap<FString, int>& MagicItemNum)
{
	//经过测试FText要进行相等比较转换成string类型后进行比较是不行的
	if (MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "HP_Medcine", "输血针")) || MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "SP_Medcine", "魔力橙")))
	{
		MagicItemName->SetText(NSLOCTEXT("Item_TEXT", "FirePaper", "招火符"));
		MagicItemImage->SetImage(&MenuStyle->ItemBrushs[0]);
		int number = MagicItemNum["FirePaper"];
		switch (number)
		{
		case 0:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "0"));
			break;
		case 1:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "1"));
			break;
		case 2:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "2"));
			break;
		default:
			break;
		}
	}
	else if (MagicItemName.IsValid() && MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "FirePaper", "招火符")))
	{
		MagicItemName->SetText(NSLOCTEXT("Item_TEXT", "LightPaper", "招雷符"));
		MagicItemImage->SetImage(&MenuStyle->ItemBrushs[1]);
		int number = MagicItemNum["LightPaper"];
		switch (number)
		{
		case 0:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number","0"));
			break;
		case 1:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "1"));
			break;
		case 2:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "2"));
			break;
		default:
			break;
		}
	}
	else if(MagicItemName.IsValid() && MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "LightPaper", "招雷符")))
	{
		MagicItemName->SetText(NSLOCTEXT("Item_TEXT", "FirePaper", "招火符"));
		MagicItemImage->SetImage(&MenuStyle->ItemBrushs[0]);
		int number = MagicItemNum["FirePaper"];
		switch (number)
		{
		case 0:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "0"));
			break;
		case 1:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "1"));
			break;
		case 2:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "2"));
			break;
		default:
			break;
		}
	}
}
void SItemWidget::SwitchMedcineItem(TMap<FString, int>& MedcineItemNum)
{
	if (MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "FirePaper", "招火符")) || MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "LightPaper", "招雷符")))
	{
		MagicItemName->SetText(NSLOCTEXT("Item_TEXT", "HP_Medcine", "输血针"));
		MagicItemImage->SetImage(&MenuStyle->ItemBrushs[2]);
		int number = MedcineItemNum["HP_Medcine"];
		switch (number)
		{
		case 0:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "0"));
			break;
		case 1:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "1"));
			break;
		case 2:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "2"));
			break;
		default:
			break;
		}
	}
	else if (MagicItemName.IsValid() && MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "HP_Medcine", "输血针")))
	{
		MagicItemName->SetText(NSLOCTEXT("Item_TEXT", "SP_Medcine", "魔力橙"));
		MagicItemImage->SetImage(&MenuStyle->ItemBrushs[3]);
		int number = MedcineItemNum["SP_Medcine"];
		switch (number)
		{
		case 0:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "0"));
			break;
		case 1:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "1"));
			break;
		case 2:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "2"));
			break;
		default:
			break;
		}
	}
	else if (MagicItemName.IsValid() && MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "SP_Medcine", "魔力橙")))
	{
		MagicItemName->SetText(NSLOCTEXT("Item_TEXT", "HP_Medcine", "输血针"));
		MagicItemImage->SetImage(&MenuStyle->ItemBrushs[2]);
		int number = MedcineItemNum["HP_Medcine"];
		switch (number)
		{
		case 0:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "0"));
			break;
		case 1:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "1"));
			break;
		case 2:
			MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "2"));
			break;
		default:
			break;
		}
	}
}
void SItemWidget::SwitchACKMode(FString ACKMode)
{
	if (ACKModeText.IsValid() && ACKMode == FString("Mid"))
		ACKModeText->SetText(NSLOCTEXT("Item_TEXT", "MidMode", "中段"));
	else if(ACKModeText.IsValid() && ACKMode == FString("High"))
		ACKModeText->SetText(NSLOCTEXT("Item_TEXT", "UpMode", "上段"));

}
void SItemWidget::ChangeItemNumber(uint8 num)
{
	switch (num)
	{
	case 0:
		MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "0"));
		break;
	case 1:
		MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "1"));
		break;
	case 2:
		MagicItemNumber->SetText(NSLOCTEXT("Item_TEXT", "Number", "2"));
		break;
	default:
		break;
	}
}
FString SItemWidget::GetCurrentItem()
{
	FString result;
	if (MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "LightPaper", "招雷符")))
		result = "LightPaper";
	else if (MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "FirePaper", "招火符")))
		result = "FirePaper";
	else if (MagicItemName->GetText().EqualTo(NSLOCTEXT("Item_TEXT", "HP_Medcine", "输血针")))
		result = "HP_Medcine";
	else
		result = "SP_Medcine";
	return result;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
