// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
//可以把这整个项目看作一个模组，在运行时首先会执行这里
#include "CoreMinimal.h"

class FSoulsLikeDemoModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};