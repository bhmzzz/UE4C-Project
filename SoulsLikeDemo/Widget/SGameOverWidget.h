// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SOULSLIKEDEMO_API SGameOverWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameOverWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
protected:
	const struct FMenuStyle* MenuStyle;

public:
	FReply EndGame();
	FReply LoadGame();
};
