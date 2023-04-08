// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoRobot.generated.h"

UCLASS()
class SOULSLIKEDEMO_API AAutoRobot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoRobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* RobotRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* RobotMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* RobotArrow;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SetRobotMovement(float Tickvalue);

	UFUNCTION()
		void OpenFire();
};
