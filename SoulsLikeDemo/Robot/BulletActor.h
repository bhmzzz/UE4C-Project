// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class SOULSLIKEDEMO_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* BulletColl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* ProjectileComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* BulletMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//当子弹距离太远时删除该对象。
	UFUNCTION()
		void DestroyWhenTooFar();
};
