// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SaveGame.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/SaveGame/MySaveGame.h"
#include "SoulsLikeDemo/MyStruct/MainPlayerController.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"

AMyPlayerState::AMyPlayerState()
{
	
}

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* controller = Cast<APlayerController>(GetOwner());
	PCIns = Cast<AMainPlayerController>(controller);
	HudIns = Cast<AMainHUD>(PCIns->GetHUD());
	player = Cast<APlayerCharacter>(controller->GetPawn());

}

void AMyPlayerState::Set_NormalMode(EPlayerStateMode& PlayerMode)
{
	PlayerMode = EPlayerStateMode::ENORMAL;
}

void AMyPlayerState::Set_EquipMode(EPlayerStateMode& PlayerMode)
{
	PlayerMode = EPlayerStateMode::EEQUIP;
}

void AMyPlayerState::Set_ACKMode(EPlayerStateMode& PlayerMode)
{
	PlayerMode = EPlayerStateMode::EACK;
}

void AMyPlayerState::Set_JumpMode(EPlayerStateMode& PlayerMode)
{
	PlayerMode = EPlayerStateMode::EJUMP;
}

void AMyPlayerState::Set_LockMode(EPlayerStateMode& PlayerMode)
{
	PlayerMode = EPlayerStateMode::ELOCK;
}

void AMyPlayerState::GetGameSaveData(UMySaveGame* Save)
{
	if (player && Save)
	{
		if (HudIns && PCIns)
		{
			PCIns->SetNormalInputMode();
		}
		player->SetActorLocation(Save->GetPlayerLocation());
	}
}
