// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightingCheckActor.generated.h"

UCLASS()
class SOULSLIKEDEMO_API ALightingCheckActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightingCheckActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* MyRootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* CheckBox;
	class ANormalEnemyCharacter* Enemy;

	bool bCheckPlayer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetActorRotationDeltaTime();

	UFUNCTION()
	void CheckPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool IsCheckPlayer();

};
