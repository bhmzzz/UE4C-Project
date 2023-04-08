// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeLightActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABladeLightActor::ABladeLightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBox"));
	SetRootComponent(RootBox);
	RootBox->SetEnableGravity(false);
	BladeLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BladeLightMesh"));
	BladeLightMesh->SetupAttachment(RootBox);
	BladeLightMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	BladeLightMesh->SetEnableGravity(false);
	BladeLightMesh->SetRelativeScale3D(FVector(0.5f, 0.f, 2.0f));
	BladeLightMesh->SetRelativeRotation(FRotator(0.f, 0.f, 30.f));
	proj = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("proj"));
	proj->InitialSpeed = 1000.f;	
	proj->MaxSpeed = 1000.f;		
	proj->ProjectileGravityScale = 0.f;	//关闭发射物组件重力
}

// Called when the game starts or when spawned
void ABladeLightActor::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<ANormalEnemyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANormalEnemyCharacter::StaticClass()));

	BladeLightMesh->OnComponentBeginOverlap.AddDynamic(this,&ABladeLightActor::CheckPlayer);
	
}

// Called every frame
void ABladeLightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DestroyWhenTooFar();
}

void ABladeLightActor::CheckPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			USoundCue* HitSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Magic/wav/BladeLightHit.BladeLightHit"));
			if (HitSound)
				UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
			if (Enemy)
				Enemy->SetPlayerHittedMontage(EMakeHitType::HitFly);
			Destroy();
		}
	}
}

void ABladeLightActor::DestroyWhenTooFar()
{
	static FVector SPawnLocation = GetActorLocation();
	if (std::abs((GetActorLocation() - SPawnLocation).Size()) >= 3000)
		Destroy();
}

