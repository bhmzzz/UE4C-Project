// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallDoor.generated.h"

UCLASS()
class SOULSLIKEDEMO_API AWallDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* DoorMesh;
	FTimerHandle handler;
	FVector WallFinalLocation = {1160.000000,1740.000000,30.000000};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void DoorMove();
	UFUNCTION()
		void SetDynamicLocation();
};