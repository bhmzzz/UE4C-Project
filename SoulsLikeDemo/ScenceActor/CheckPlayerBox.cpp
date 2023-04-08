// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPlayerBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/Widget/SEnemyWidget.h"
#include "SoulsLikeDemo/ScenceActor/WallDoor.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"

// Sets default values
ACheckPlayerBox::ACheckPlayerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckBox"));
}

// Called when the game starts or when spawned
void ACheckPlayerBox::BeginPlay()
{
	Super::BeginPlay();
	CheckBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckPlayerBox::CheckPlayer);
}

// Called every frame
void ACheckPlayerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckPlayerBox::CheckPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(),ANormalEnemyCharacter::StaticClass()));
			if (AI)
			{
				AI->SeePlayer();
			}
			AWallDoor* WallDoor = Cast<AWallDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), AWallDoor::StaticClass()));
			WallDoor->DoorMove();
			AMainHUD* MainHUD =  Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
			if (MainHUD->GetEnemyWidget().IsValid())
			{
				MainHUD->AddEnemyWidgetToViewPort();
				GetWorldTimerManager().SetTimer(handler, this, &ACheckPlayerBox::DestroySelf,GetWorld()->DeltaTimeSeconds, true);
				EnemyWidget = MainHUD->GetEnemyWidget();
				EnemyWidget->ProgressBarChange();
			}
		}
	}
}

void ACheckPlayerBox::DestroySelf()
{
	if (EnemyWidget->GetProgressBarFullBoolean())
	{
		Destroy();
		GetWorldTimerManager().ClearTimer(handler);
	}
}

