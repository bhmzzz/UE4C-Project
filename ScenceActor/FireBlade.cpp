// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBlade.h"
#include "Components/BoxComponent.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/MyStruct/MainPlayerController.h"

// Sets default values
AFireBlade::AFireBlade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BladeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BladeMesh"));
	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBox"));
	collisionBox->SetupAttachment(BladeMesh);
	ScenceUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScenceUI"));
	ScenceUI->SetupAttachment(BladeMesh);
	ScenceUI->SetVisibility(false);

	player = NULL;
}

// Called when the game starts or when spawned
void AFireBlade::BeginPlay()
{
	Super::BeginPlay();

	collisionBox->OnComponentBeginOverlap.AddDynamic(this,&AFireBlade::OpenFireBalde);
	collisionBox->OnComponentEndOverlap.AddDynamic(this, &AFireBlade::PlayerOut);
}

// Called every frame
void AFireBlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireBlade::OpenFireBalde(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			ScenceUI->SetVisibility(true);
			GetWorldTimerManager().SetTimer(CheckFHandler, this, &AFireBlade::PlayerPressF, GetWorld()->DeltaTimeSeconds, true);
		}
	}
}

void AFireBlade::PlayerOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			ScenceUI->SetVisibility(false);
			GetWorldTimerManager().ClearTimer(CheckFHandler);
		}
	}
}



void AFireBlade::PlayerPressF()
{
	//打开篝火交互UI
	if (player && player->GetPressF())
	{
		AMainPlayerController* pc = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (pc)
			pc->OpenFireBladeUI();
		GetWorldTimerManager().ClearTimer(CheckFHandler);
	}

}

