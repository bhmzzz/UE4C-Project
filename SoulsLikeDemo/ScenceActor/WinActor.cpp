// Fill out your copyright notice in the Description page of Project Settings.


#include "WinActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/MyStruct/MainPlayerController.h"

// Sets default values
AWinActor::AWinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootComponent"));
	SetRootComponent(MyRootComponent);
	CheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckBox"));
	CheckBox->SetupAttachment(MyRootComponent);
	CheckBox->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	CheckBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
	CheckBox->bHiddenInGame = true;
	UIWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UIWidget"));
	UIWidget->SetupAttachment(MyRootComponent);
	
}

// Called when the game starts or when spawned
void AWinActor::BeginPlay()
{
	Super::BeginPlay();

	/*CheckBox->OnComponentBeginOverlap.AddDynamic(this, &AWinActor::CheckPlayerAndOpenUI);*/
}

// Called every frame
void AWinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWinActor::OpenUI()
{
	AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	AMainPlayerController* PC = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (HudIns && PC)
	{
		HudIns->AddWinWidgetToViewPort();
		PC->SetUIInputMode();
		Destroy();
	}
}

//void AWinActor::CheckPlayerAndOpenUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	DebugNS::MyDebug(1.f, "Success");
//	if (OtherActor)
//	{
//		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
//		if (player)
//		{
//			FTimerDelegate CheckDelegate;
//			CheckDelegate.BindUObject(this, &AWinActor::OpenOverUI, player);
//			GetWorldTimerManager().SetTimer(CheckPressFHandler, CheckDelegate, GetWorld()->DeltaTimeSeconds, true);
//		}
//	}
//}
//
//void AWinActor::OpenOverUI(APlayerCharacter* player)
//{
//	if (player->GetPressF())
//	{
//		GetWorldTimerManager().ClearTimer(CheckPressFHandler);
//		AMainHUD* HudIns = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
//		if (HudIns)
//			HudIns->AddWinWidgetToViewPort();
//	}
//}

