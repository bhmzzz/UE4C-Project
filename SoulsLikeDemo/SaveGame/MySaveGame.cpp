// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/Character.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

//静态成员类外初始化
UMySaveGame* UMySaveGame::SaveGameIns = nullptr;

UMySaveGame::UMySaveGame()
{
	PlayerLocation = FVector(0.f,0.f,0.f);
}

void UMySaveGame::SavePlayerLocation(FVector location)
{
	PlayerLocation = location;
}

UMySaveGame* UMySaveGame::GetSaveGameIns()
{
	if(SaveGameIns == nullptr)
		SaveGameIns = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(StaticClass()));
	return SaveGameIns;
}

FVector UMySaveGame::GetPlayerLocation()
{
	return PlayerLocation;
}
