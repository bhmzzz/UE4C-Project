// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

UCLASS()
class SOULSLIKEDEMO_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* DoorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* collisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* TextWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bPlayerIn;

	FTimerHandle handler1;
	FTimerHandle handler2;

	bool bIsopen;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void CheckPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void CheckOpenDoor();
	UFUNCTION()
		void OpenDoor();
};
