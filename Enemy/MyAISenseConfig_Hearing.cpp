// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAISenseConfig_Hearing.h"

UMyAISenseConfig_Hearing::UMyAISenseConfig_Hearing(const FObjectInitializer& ObjectInitializer)
{
	HearingRange = 1500.f;
	//LoSHearingRange = 2000.f;	//调试器中显示听觉范围的不同半径
	//bUseLoSHearing = true;
	DetectionByAffiliation.bDetectEnemies = true;
	DetectionByAffiliation.bDetectFriendlies = true;
	DetectionByAffiliation.bDetectNeutrals = true;
	bStartsEnabled = true;
	Implementation = UAISenseConfig_Hearing::StaticClass();
}
