// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScenceCamera.generated.h"

UCLASS()
class SOULSLIKEDEMO_API AScenceCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScenceCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UCameraComponent* ScenceCamera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
