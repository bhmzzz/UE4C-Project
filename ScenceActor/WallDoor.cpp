// Fill out your copyright notice in the Description page of Project Settings.


#include "WallDoor.h"
#include "Kismet/KismetMathLibrary.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

// Sets default values
AWallDoor::AWallDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
}

// Called when the game starts or when spawned
void AWallDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallDoor::DoorMove()
{
	GetWorldTimerManager().SetTimer(handler, this, &AWallDoor::SetDynamicLocation, GetWorld()->DeltaTimeSeconds, true);
}

void AWallDoor::SetDynamicLocation()
{
	if (GetActorLocation().Equals(WallFinalLocation))
		GetWorldTimerManager().ClearTimer(handler);
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), WallFinalLocation, GetWorld()->DeltaTimeSeconds, 1.f));
}

