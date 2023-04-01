// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissileActor.generated.h"

UCLASS()
class SOULSLIKEDEMO_API AMissileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MissileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UArrowComponent* Arrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "MissileCount")
		int count = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector> SamplePoint;
	FVector CurrentLocation = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasHitEnemy;
	
	FTimerHandle BezierCurveHandler; //贝塞尔曲线时间句柄。
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//这里固定做四个点的贝塞尔曲线，点位置由函数进行动态计算
	UFUNCTION(BlueprintCallable)
		TArray<FVector> GetBezierCurveSamplePoint();
	UFUNCTION(BlueprintCallable)
		FVector BezierCurvePath(TArray<FVector> SamplePoints,float alpha);
	UFUNCTION()
		void DestroyThisActor();
	UFUNCTION()
		void MissileExplosion();
	UFUNCTION()
		void FindMissileFlyCollision();
	UFUNCTION()
		void InitSocketLocation();
	/*UFUNCTION()
		bool MissileBoxTraceCheckActor(FVector HalfSize, FVector StartLocation, FVector EndLocation, FRotator Rotation);*/
};
