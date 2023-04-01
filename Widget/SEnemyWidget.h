// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FCurveSequence ProgressbarAnim;
public:
	void ProgressBarChange();

	bool GetProgressBarFullBoolean();

	void ChangeAI_OptionBar(TMap<AI_Option::Type,float> AIOptionValue);

};
