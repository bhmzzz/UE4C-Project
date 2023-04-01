// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Door(TEXT("/Game/StarterContent/Props/SM_Door"));
	if (Door.Succeeded())
		DoorMesh->SetStaticMesh(Door.Object);
	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBox"));
	collisionBox->SetupAttachment(DoorMesh);
	collisionBox->SetRelativeLocation(FVector(40.f, -40.f, 70.f));
	collisionBox->SetBoxExtent(FVector(32.f, 32.f, 64.f));
	TextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TextWidget"));
	TextWidget->SetupAttachment(DoorMesh);
	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::CheckPlayer);
	TextWidget->SetVisibility(false);
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorActor::CheckPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsopen)
	{
		if (OtherActor)
		{
			APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
			if (player)
			{
				player->OverShoulderView();
				TextWidget->SetVisibility(true);
				bPlayerIn = true;
				GetWorldTimerManager().SetTimer(handler1, this, &ADoorActor::CheckOpenDoor, 0.1f, true);
			}
		}
	}
}

void ADoorActor::CheckOpenDoor()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player->GetPressF())
	{
		bIsopen = true;
		GetWorldTimerManager().ClearTimer(handler1);
		player->ResetCameraLocation();
		GetWorldTimerManager().SetTimer(handler1, this, &ADoorActor::OpenDoor, GetWorld()->DeltaTimeSeconds, true);
	}
}

void ADoorActor::OpenDoor()
{
	static FRotator FinalRotation = DoorMesh->GetRelativeRotation() + FRotator(0.f,-90.f,0.f);
	if (DoorMesh->GetRelativeRotation().Equals(FinalRotation))
		GetWorldTimerManager().ClearTimer(handler2);
	SetActorRelativeRotation(UKismetMathLibrary::RInterpTo(DoorMesh->GetRelativeRotation(), FinalRotation, GetWorld()->DeltaTimeSeconds,10.f));
}

