// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpAttackEffectActor.generated.h"

UCLASS()
class SOULSLIKEDEMO_API AJumpAttackEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpAttackEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle handler1;	//destoryActor handler

public:	
	// Called every frame
	// Called every frame
	virtual void Tick(float DeltaTime) override
	{
		Super::Tick(DeltaTime);

	}

	UFUNCTION()
		void PlayParticlesEffect();
	UFUNCTION()
		void CallFunction();
	UFUNCTION()
		void MyDestroyActor();

};
