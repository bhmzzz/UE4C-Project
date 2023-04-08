// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
}

float UPlayerAnimInstance::UpdateSpeed()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	float Speed = 0;
	if (Player)
	{
		PlayerSpeed = Player->GetVelocity().Size();
		Speed = PlayerSpeed;
	}
	return Speed;
}

float UPlayerAnimInstance::UpdateDirection()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	float Direction = 0;
	if (Player)
	{
		PlayerDirection = CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());
		Direction = PlayerDirection;
	}
	return Direction;
}

void UPlayerAnimInstance::UpdatePlayerInput()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		InputX = Player->GetInputX();
		InputY = Player->GetInputY();
	}
}

bool UPlayerAnimInstance::CanAirACK()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		if (Player->GetDownACKBool())
			return true;
		else
			return false;
	}
	return false;
}
