// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SOULSLIKEDEMO_API SFireBladeWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFireBladeWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<SButton> CloseButton;

	TSharedPtr<SButton> SkillButton;

private:
	const struct FMenuStyle* MenuStyle;

public:
	FReply CloseBtn_OnCLicked();
	FReply SkillBtn_OnCLicked();
	FReply SaveGameBtn_OnClicked();
};
