﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinActor.generated.h"

UCLASS()
class SOULSLIKEDEMO_API AWinActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWinActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* MyRootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* CheckBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* UIWidget;

	FTimerHandle CheckPressFHandler;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*void CheckPlayerAndOpenUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OpenOverUI(class APlayerCharacter* player);*/
	UFUNCTION(BlueprintCallable)
		void OpenUI();

};
