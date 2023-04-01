// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestSkillActor.generated.h"

UCLASS()
class SOULSLIKEDEMO_API ATestSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestSkillActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* Skillbox;

	class APlayerCharacter* Player;

	//UParticleSystem* ParticleTemplate;

	FTimerHandle handler1;	//destoryActor handler

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void PlayParticlesEffect();
	UFUNCTION()
		void CallFunction();
	UFUNCTION()
		void MyDestroyActor();

};
