// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyPlayerState::AMyPlayerState()
{
	
}

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* controller = Cast<APlayerController>(GetOwner());
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
