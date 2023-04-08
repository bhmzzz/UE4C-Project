// Fill out your copyright notice in the Description page of Project Settings.


#include "LightingCheckActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

// Sets default values
ALightingCheckActor::ALightingCheckActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootComponent"));
	SetRootComponent(MyRootComponent);
	CheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckBox"));
	CheckBox->SetupAttachment(MyRootComponent);
	CheckBox->SetRelativeScale3D(FVector(100.f, 2.f, 1.5f));
	CheckBox->bHiddenInGame = true;
	//CheckBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CheckBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void ALightingCheckActor::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<ANormalEnemyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANormalEnemyCharacter::StaticClass()));

	CheckBox->OnComponentBeginOverlap.AddDynamic(this, &ALightingCheckActor::CheckPlayer);
}

// Called every frame
void ALightingCheckActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotationDeltaTime();
}

void ALightingCheckActor::SetActorRotationDeltaTime()
{
	static ANormalEnemyCharacter* AIIns = Cast<ANormalEnemyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANormalEnemyCharacter::StaticClass()));
	if (AIIns)
	{
		FRotator NewRotator(GetActorRotation().Pitch, AIIns->GetActorRotation().Yaw, GetActorRotation().Roll);
		SetActorRotation(NewRotator);
	}
}

void ALightingCheckActor::CheckPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player && Enemy)
		{
			bCheckPlayer = true;
		}
	}
}

bool ALightingCheckActor::IsCheckPlayer()
{
	return bCheckPlayer;
}



