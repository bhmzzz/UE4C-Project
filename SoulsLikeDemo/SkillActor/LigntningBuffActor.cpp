// Fill out your copyright notice in the Description page of Project Settings.


#include "LigntningBuffActor.h"

// Sets default values
ALigntningBuffActor::ALigntningBuffActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));*/
}

// Called when the game starts or when spawned
void ALigntningBuffActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALigntningBuffActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

