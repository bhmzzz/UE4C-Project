// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SOULSLIKEDEMO_API SMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMenuWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	bool bClickGameStart;

protected:

private:
	//获取Menu样式
	const struct FMenuStyle* MenuStyle;
	struct FButtonStyle* NewButtonStyle;

public:
	FReply GameStart_OnClicked();

};
