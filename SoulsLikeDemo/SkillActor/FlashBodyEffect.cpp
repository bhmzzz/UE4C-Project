// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashBodyEffect.h"
#include "Components/PoseableMeshComponent.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "SoulsLikeDemo/Common/GameDataType.h"

// Sets default values
AFlashBodyEffect::AFlashBodyEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PoseableMeshComp = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMeshComp"));
	PoseableMeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFlashBodyEffect::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	USkeletalMesh* ShadowMesh = LoadObject<USkeletalMesh>(NULL, TEXT("ShadowMesh'/Game/LP287Character_P3/Meshes/Characters/Combines/SK_LP287_P3C2.SK_LP287_P3C2'"));
	PoseableMeshComp->SetSkeletalMesh(ShadowMesh);

	if (player && PoseableMeshComp && ShadowMesh)	//设置动态动作网格体
		PoseableMeshComp->CopyPoseFromSkeletalComponent(player->GetMesh());

	if (player->GetPlayerFlashType() == EPlayerFlashType::EClosetFlash)
	{
		Material = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/MyMaterial/FlashShodow.FlashShodow'"));
		TArray<UMaterialInterface*> MaterialArr = PoseableMeshComp->GetMaterials();
		if (Material && MaterialArr.Num() != 0)
		{
			for (int i = 0; i < MaterialArr.Num(); ++i)
				PoseableMeshComp->SetMaterial(i, Material);
		}

		GetWorldTimerManager().SetTimer(TimeLineHandler, this, &AFlashBodyEffect::ClosetFlashTimerEnd, 1.f, false);
	}
	else if (player->GetPlayerFlashType() == EPlayerFlashType::EFlashBody)
	{
		Material = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/MyMaterial/FlashBody.FlashBody'"));

		TArray<UMaterialInterface*> MaterialArr = PoseableMeshComp->GetMaterials();
		if (Material && MaterialArr.Num() != 0)
		{
			for (int i = 0; i < MaterialArr.Num(); ++i)
				PoseableMeshComp->SetMaterial(i, Material);
		}

		GetWorldTimerManager().SetTimer(TimeLineHandler, this, &AFlashBodyEffect::FlashBodyTimeOut, 0.3f, false);
	}
}

// Called every frame
void AFlashBodyEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlashBodyEffect::ClosetFlashTimerEnd()
{
	GetWorldTimerManager().ClearTimer(TimeLineHandler);
	Destroy();
}

void AFlashBodyEffect::FlashBodyTimeOut()
{
	GetWorldTimerManager().ClearTimer(TimeLineHandler);
	Destroy();
}



