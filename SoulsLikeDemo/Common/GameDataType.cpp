// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataType.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Robot/MissileActor.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "Kismet/GameplayStatics.h"

void DebugNS::MyDebug(float DurationTime, FString Message)
{
		GEngine->AddOnScreenDebugMessage(-1, DurationTime, FColor::Green, Message);
}

void DebugNS::DurationDebug(float DurationTime, FString Message)
{
		GEngine->AddOnScreenDebugMessage(0, DurationTime, FColor::Green, Message);
}

bool ProbabilityFUNC(float value)
{
	float temp = UKismetMathLibrary::RandomFloatInRange(1, 100);
	bool bResult;
	if (value >= temp)
		bResult = true;
	else
		bResult = false;
	return bResult;
}

USoundCue* GameDataType::HittedSound = nullptr;
GameDataType::GameDataType()
{
	/*static ConstructorHelpers::FObjectFinder<USoundCue> HittedSoundTarget(TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Weapons/wav/HitedCue"));
	if (HittedSoundTarget.Succeeded())
	{
		HittedSound = HittedSoundTarget.Object;
	}*/
}

void GameDataType::PlayHittedSound(const UObject* WorldContextObject)
{
	if (HittedSound)
	{
		UGameplayStatics::PlaySound2D(WorldContextObject, HittedSound);
	}
	else
	{
		DebugNS::MyDebug(1.f, "NotSound");
	}
}