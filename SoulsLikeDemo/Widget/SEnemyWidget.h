// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class SOULSLIKEDEMO_API SEnemyWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEnemyWidget)
	{}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
protected:
	bool bProgressBarFull;
private:
	TSharedPtr<class SProgressBar> Enemy_HPB;
	TSharedPtr<SProgressBar> Enemy_AMB;
	TSharedPtr<class STextBlock> BT_Text;
	TSharedPtr<STextBlock> JQ_Text;
	TSharedPtr<STextBlock> LJ_Text;
	TSharedPtr<STextBlock> RS_Text;
	FCurveSequence ProgressbarAnim;
public:
	void ProgressBarChange();

	bool GetProgressBarFullBoolean();

	void ChangeAI_OptionBar(TMap<AI_Option::Type,float> AIOptionValue);

	void ChangeAI_State(EBossStateType stateArr);

	void SetDebuffNone(EBossStateType Debuff);

};
