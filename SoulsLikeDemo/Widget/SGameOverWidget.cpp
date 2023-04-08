// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameOverWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "MenuStyleHelper.h"
#include "MenuWidgetStyle.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/PlayerCharacter/MyPlayerState.h"
#include "SoulsLikeDemo/SaveGame/MySaveGame.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGameOverWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &MenuStyleHelper::Get().GetWidgetStyle<FMenuStyle>("MenuStyle");
	ChildSlot
		[
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f, 0.f, 0.5f, 0.f))
		.Offset(FMargin(0.f, 200.f, 550.f, 150.f))
		.Alignment(FVector2D(0.5f, 0.f))
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("GameOverText","GameOver","游戏结束"))
			.ColorAndOpacity(FLinearColor(1.f,0.3f,0.5f,1.f))
			.Font(MenuStyle->GameTitleFont)
			.Justification(ETextJustify::Center)
		]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f))
		.Offset(FMargin(0.f,0.f,300.f,200.f))
		.Alignment(FVector2D(0.5f,0.5f))
		[
			SNew(SVerticalBox)
			/*+ SVerticalBox::Slot()
			.Padding(FMargin(60.f,40.f,60.f,0.f))
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SButton)
				.OnClicked(this,&SGameOverWidget::LoadGame)
				.ContentPadding(FMargin(4.f,2.f))
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("GameOverText","LoadSave","读取存档"))
					.Font(MenuStyle->CommonFont)
					.Justification(ETextJustify::Center)
				]
			]*/
			+ SVerticalBox::Slot()
			[
				SNew(SSpacer)
				.Size(FVector2D(1.f,20.f))
			]
			+ SVerticalBox::Slot()
				.Padding(FMargin(60.f, 40.f, 60.f, 0.f))
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
				SNew(SButton)
				.OnClicked(this, &SGameOverWidget::EndGame)
				.ContentPadding(FMargin(4.f, 2.f))
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("GameOverText", "EndGame", "结束游戏"))
					.Font(MenuStyle->CommonFont)
					.Justification(ETextJustify::Center)
				]
			]
		]
	];
}
FReply SGameOverWidget::EndGame()
{
	UWorld* world = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	UKismetSystemLibrary::ExecuteConsoleCommand(world, "Exit");
	return FReply::Handled();
}
FReply SGameOverWidget::LoadGame()
{
	UWorld* world = UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld();
	UMySaveGame* MySave = UMySaveGame::GetSaveGameIns();
	MySave = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MySave"), 0));
	AMyPlayerState* psIns = UGameplayStatics::GetPlayerController(world, 0)->GetPlayerState<AMyPlayerState>();
	if (MySave && psIns)
		psIns->GetGameSaveData(MySave);
	return FReply::Handled();;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
