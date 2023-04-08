// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SOULSLIKEDEMO_API SWinWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWinWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
protected:
	const struct FMenuStyle* MenuStyle;
	FReply EndGame();
};
