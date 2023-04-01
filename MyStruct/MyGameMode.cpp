// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "SoulsLikeDemo/PlayerCHaracter/PlayerCharacter.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"
#include "MainPlayerController.h"
#include "SoulsLikeDemo/PlayerCharacter/MyPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/ScenceActor/ScenceCamera.h"
#include "GameFramework/PlayerController.h"
//#include "SoulsLikeDemo/Common/GameDataType.h"


AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = AMainHUD::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();

	CurrentGameMode = EGAME_MODE::Type::ENULL;
}

void AMyGameMode::BeginPlay()
{
	AActor* cameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), AScenceCamera::StaticClass());
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (cameraActor && PC)
		PC->SetViewTarget(cameraActor);

}

EGAME_MODE::Type AMyGameMode::GetCurrentGameMode()
{
	return CurrentGameMode;
}

void AMyGameMode::SetCurrentGameMode(EGAME_MODE::Type ChangeMode)
{
	switch (ChangeMode)
	{
	case EGAME_MODE::Type::EMenuUI:
		CurrentGameMode = EGAME_MODE::Type::EMenuUI;
		break;
	case EGAME_MODE::Type::EInGame:
		CurrentGameMode = EGAME_MODE::Type::EInGame;
		break;
	case EGAME_MODE::Type::EGamePause:
		CurrentGameMode = EGAME_MODE::Type::EGamePause;
		break;
	case EGAME_MODE::Type::EGameEnd:
		CurrentGameMode = EGAME_MODE::Type::EGameEnd;
		break;
	default:
		break;
	}
}
