// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAISenseConfig_Sight.h"

UMyAISenseConfig_Sight::UMyAISenseConfig_Sight(const FObjectInitializer& ObjectInitializer)
{
	SightRadius = 1500;
	LoseSightRadius = 2000;
	PeripheralVisionAngleDegrees = 30;
	DetectionByAffiliation.bDetectEnemies = true;
	DetectionByAffiliation.bDetectFriendlies = true;
	DetectionByAffiliation.bDetectNeutrals = true;
	bStartsEnabled = true;
	Implementation = UAISense_Sight::StaticClass();
}
