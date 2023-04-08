// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "NormalEnemyCharacter.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	
}

float UEnemyAnimInstance::UpdateDate()
{
	this->Speed = 0.0f;
	ANormalEnemyCharacter* enemyAI;
	enemyAI = Cast<ANormalEnemyCharacter>(TryGetPawnOwner());
	if (enemyAI)
		this->Speed = enemyAI->GetVelocity().Size();
	return this->Speed;
}

