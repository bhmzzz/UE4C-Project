// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckPlayerBox.generated.h"

UCLASS()
class SOULSLIKEDEMO_API ACheckPlayerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPlayerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* CheckBox;
	FTimerHandle handler;
	TSharedPtr<class SEnemyWidget> EnemyWidget;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void CheckPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void DestroySelf();

};
