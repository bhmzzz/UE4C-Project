// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LigntningBuffActor.generated.h"

UCLASS()
class SOULSLIKEDEMO_API ALigntningBuffActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALigntningBuffActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
		class UNiagaraComponent* NiagaraComp;*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
