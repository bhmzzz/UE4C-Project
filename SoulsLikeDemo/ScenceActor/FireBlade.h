// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireBlade.generated.h"

UCLASS()
class SOULSLIKEDEMO_API AFireBlade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireBlade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		UStaticMeshComponent* BladeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* collisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* ScenceUI;

	FTimerHandle CheckFHandler;

	class APlayerCharacter* player;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OpenFireBalde(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void PlayerOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void PlayerPressF();

};
