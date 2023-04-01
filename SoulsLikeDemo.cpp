// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoulsLikeDemo.h"
#include "Modules/ModuleManager.h"
#include "SlateBasics.h"
#include "SoulsLikeDemo/Widget/MenuStyleHelper.h"



void FSoulsLikeDemoModule::StartupModule()
{
	//初始化样式
	FSlateStyleRegistry::UnRegisterSlateStyle(MenuStyleHelper::GetStyleSetName());
	MenuStyleHelper::Initialze();
}

void FSoulsLikeDemoModule::ShutdownModule()
{
	MenuStyleHelper::shutDown();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FSoulsLikeDemoModule, SoulsLikeDemo, "SoulsLikeDemo");
