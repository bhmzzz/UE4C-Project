// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"
#include "MenuWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct SOULSLIKEDEMO_API FMenuStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FMenuStyle();
	virtual ~FMenuStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FMenuStyle& GetDefault();

	UPROPERTY(EditAnywhere)
		FSlateBrush MenuTopBrush;
	UPROPERTY(EditAnywhere)
		FSlateBrush ButtonBrush;
	UPROPERTY(EditAnywhere)
		FSlateBrush MenuBGBrush;
	UPROPERTY(EditAnywhere)
		FSlateBrush BGBrush;
	UPROPERTY(EditAnywhere)
		FSlateBrush ItemBorderBrush;
	UPROPERTY(EditAnywhere)
		TArray<FSlateBrush> ItemBrushs;
	UPROPERTY(EditAnywhere)
		FSlateFontInfo CommonFont;
	UPROPERTY(EditAnywhere)
		FSlateFontInfo GameTitleFont;
	UPROPERTY(EditAnywhere)
		FSlateFontInfo MenuTitleFont;
	UPROPERTY(EditAnywhere)
		FSlateFontInfo ItemNumberFont;
	UPROPERTY(EditAnywhere)
		FSlateFontInfo ItemNameFont;
	UPROPERTY(EditAnywhere)
		FSlateSound HitButtonSound;
	UPROPERTY(EditAnywhere)
		FSlateFontInfo ACKModeFont;
	UPROPERTY(EditAnywhere)
		TMap<ESkillRadioType, FSlateBrush> SkillBrushs;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UMenuWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FMenuStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
