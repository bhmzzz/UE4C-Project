// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuStyleHelper.h"
#include "Slate/SlateGameResources.h"

TSharedPtr<FSlateStyleSet> MenuStyleHelper::MenuStyleInstance = NULL;

void MenuStyleHelper::Initialze()
{
	if (!MenuStyleInstance.IsValid()) {
		MenuStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*MenuStyleInstance);
	}
}

FName MenuStyleHelper::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MenuStyle"));
	return StyleSetName;
}

void MenuStyleHelper::shutDown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*MenuStyleInstance);
	ensure(MenuStyleInstance.IsUnique());
	MenuStyleInstance.Reset();
}

const ISlateStyle& MenuStyleHelper::Get()
{
	return *MenuStyleInstance;
}

TSharedRef<class FSlateStyleSet> MenuStyleHelper::Create()
{
	//在蓝图路径中找到该资源并对指针进行赋值
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(GetStyleSetName(),"/Game/UI", "/Game/UI");
	return StyleRef;
}
