// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SOULSLIKEDEMO_API SSkillWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSkillWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<SButton> CloseButton;

	TSharedPtr<SImage> AnimImage;

	TSharedPtr<STextBlock> SkillIntroduce;

	class UMediaPlayer* CurrentMedia;

private:
	const struct FMenuStyle* MenuStyle;

public:
	FReply CloseSkillWidget();
	FReply PlayUpAnim();
	FReply PlayMidAnim();
	FReply PlaySlideAnim();
	FReply PlayFlashAnim();
	FReply PlayCloseFlashAnim();
	FReply PlayAirACKAnim();
	FReply PlayCYZAnim();
	FReply PlayJuHeAnim();
	FReply PlayDownACKAnim();
	FReply PlayUpperAnim();
	FReply PlayOpenFireAnim();
	FReply PlayMissileAnim();
	FReply PlaySkillPointsAnim();
	void PlaySkillAnimRadio(UMediaPlayer* Media, UFileMediaSource* Source);
};
