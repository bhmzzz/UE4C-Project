// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

/**
 * 
 */
//该类作用为获取编辑器中蓝图的样式，将style注册进游戏，该类为单例
class SOULSLIKEDEMO_API MenuStyleHelper
{
public:

	static void Initialze();
	//获取到注册进游戏的名字
	static FName GetStyleSetName();

	static void shutDown();
	//提供给外部获取该单例的接口
	static const ISlateStyle& Get();
	
private:
	//用于注册样式
	static TSharedRef<class FSlateStyleSet> Create();

	//单例下此变量不能在构造函数中实现
	static TSharedPtr<FSlateStyleSet> MenuStyleInstance;
};
