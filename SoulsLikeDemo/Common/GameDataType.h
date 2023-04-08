// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/KismetMathLibrary.h"
#include "CoreMinimal.h"
#include "Sound/SoundCue.h"

/**
 * 
 */

namespace MissileCount {
	int count = 0;
}

//自定义Debug函数
namespace DebugNS {
	void MyDebug(float DurationTime, FString Message);

	void DurationDebug(float DurationTime, FString Message);
};

//AI的负面Debuff
namespace EAIDebuff {
	enum class Type : uint8
	{
		ELightning,	//雷击Debuff
		EPoisoning	//中毒Debuff
	};
}

//概率判断函数
bool ProbabilityFUNC(float value);

//AI数值属性枚举
namespace AI_Option {
	enum class Type : uint8
	{
		EHP, //AI HP
		EAM // AI Ammor
	};
}

namespace EGAME_MODE {
	enum class Type : uint8
	{
		ENULL,
		EMenuUI,
		EInGame,
		EGamePause,
		EGameEnd
	};
}

class SOULSLIKEDEMO_API GameDataType
{
public:
	GameDataType();
	
	static void PlayHittedSound(const UObject* WorldContextObject);
private:
	static class USoundCue* HittedSound;
};
