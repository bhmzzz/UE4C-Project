// Fill out your copyright notice in the Description page of Project Settings.


#include "SWinWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "MenuStyleHelper.h"
#include "MenuWidgetStyle.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SWinWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &MenuStyleHelper::Get().GetWidgetStyle<FMenuStyle>("MenuStyle");
	ChildSlot
	[
		// Populate the widget
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f, 0.f, 0.5f, 0.f))
		.Offset(FMargin(0.f, 200.f, 550.f, 150.f))
		.Alignment(FVector2D(0.5f, 0.f))
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("WinText", "GameOver", "游戏结束"))
			.ColorAndOpacity(FLinearColor(1.f, 0.3f, 0.5f, 1.f))
			.Font(MenuStyle->GameTitleFont)
			.Justification(ETextJustify::Center)
		]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f))
		.Offset(FMargin(0.f,0.f,150.f,50.f))
		.Alignment(FVector2D(0.5f))
		[
			SNew(SButton)
			.OnClicked(this,&SWinWidget::EndGame)
			.ContentPadding(FMargin(4.f,2.f))
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(NSLOCTEXT("WinText","EndGame","结束游戏"))
				.Font(MenuStyle->CommonFont)
			]
		]
	];
	
}
FReply SWinWidget::EndGame()
{
	UWorld* world = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	UKismetSystemLibrary::ExecuteConsoleCommand(world, "Exit");
	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
