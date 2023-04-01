// Fill out your copyright notice in the Description page of Project Settings.


#include "ScenceCamera.h"

// Sets default values
AScenceCamera::AScenceCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ScenceCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ScenceCamera"));
}

// Called when the game starts or when spawned
void AScenceCamera::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(-3230.000000, 2260.000000, 160.000000));
	SetActorRotation(FRotator(0.f, -120.f, 0.f));
}

// Called every frame
void AScenceCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

