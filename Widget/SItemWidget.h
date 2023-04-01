// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SOULSLIKEDEMO_API SItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SItemWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<class STextBlock> MagicItemName;
	TSharedPtr<class SImage> MagicItemImage;
	TSharedPtr<STextBlock> MagicItemNumber;
	TSharedPtr<STextBlock> ACKModeText;

private:
	const struct FMenuStyle* MenuStyle;

public:
	void SwitchMagicItem(TMap<FString,int>& MagicItemNum);
	void SwitchACKMode(FString ACKMode);
};
