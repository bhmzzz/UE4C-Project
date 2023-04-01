// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPlayerWidget::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(TAttribute<FAnchors>(FAnchors(0.5f,0.5f)))
		.Offset(FMargin(-475.0f,-16.0f,370.0f,3.0f))
		[
			SNew(SProgressBar)
			.Percent(0.5f)
			.RenderTransformPivot(TAttribute<FVector2D>(FVector2D(0.5f,0.5f)))

		]
	   + SConstraintCanvas::Slot()
	   .Anchors(TAttribute<FAnchors>(FAnchors(0.5f, 0.5f)))
	   .Offset(FMargin(-416.0f, -32.0f, 260.f, 15.f))
		[
			SNew(SHorizontalBox)
			.RenderTransformPivot(TAttribute<FVector2D>(FVector2D(0.5f, 0.5f)))
		    + SHorizontalBox::Slot()
		    .HAlign(EHorizontalAlignment::HAlign_Center)
		    .VAlign(EVerticalAlignment::VAlign_Center)
		    [
				SNew(STextBlock)
				.Text(TAttribute<FText>(FText::FromString("Health:")))
				.ColorAndOpacity(TAttribute<FSlateColor>(FLinearColor(0.028368,0.053568,0.158333,1.000000)))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold",8))
			]
		    + SHorizontalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock)
				.Text(TAttribute<FText>(FText::FromString("99%")))
				.ColorAndOpacity(TAttribute<FSlateColor>(FLinearColor(0.028368, 0.053568, 0.158333, 1.000000)))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
			]
			+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(TAttribute<FText>(FText::FromString("Enegy:")))
				.ColorAndOpacity(TAttribute<FSlateColor>(FLinearColor(0.028368, 0.053568, 0.158333, 1.000000)))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
				]
			+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(TAttribute<FText>(FText::FromString("99%")))
				.ColorAndOpacity(TAttribute<FSlateColor>(FLinearColor(0.028368, 0.053568, 0.158333, 1.000000)))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
				]
		]
	   
		
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
