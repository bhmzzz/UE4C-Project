// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlashBodyEffect.generated.h"

UCLASS()
class SOULSLIKEDEMO_API AFlashBodyEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlashBodyEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPoseableMeshComponent* PoseableMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyMaterial")
	class UMaterialInterface* Material;

	FTimerHandle TimeLineHandler;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void ClosetFlashTimerEnd();

	UFUNCTION()
		void FlashBodyTimeOut();

};
