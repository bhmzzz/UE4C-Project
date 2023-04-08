// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoRobot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "Components/ArrowComponent.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "BulletActor.h"

// Sets default values
AAutoRobot::AAutoRobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RobotRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RobotRootComponent"));
	RootComponent = RobotRootComponent;
	
	RobotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RobotMesh"));
	RobotMesh->SetupAttachment(RootComponent);

	RobotArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RobotArrow"));
	RobotArrow->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RobotMeshAssert(TEXT("/Engine/VREditor/BasicMeshes/SM_Cube_01"));
	if (RobotMeshAssert.Succeeded())
	{
		RobotMesh->SetStaticMesh(RobotMeshAssert.Object);	//在编辑器中找到对应的StaticMesh分配给RobotMesh。
		SetActorScale3D(FVector(0.5f,0.5f,0.5f));
	}
	//设置mesh的碰撞为NoCollision
	static const FName RobotCollisionProfileFileName(TEXT("NoCollision"));
	RobotMesh->BodyInstance.SetCollisionProfileName(RobotCollisionProfileFileName);

	RobotMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AAutoRobot::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAutoRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//设置Robot上下悬浮
	SetRobotMovement(GetWorld()->GetTimeSeconds());

}

void AAutoRobot::SetRobotMovement(float Tickvalue)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	static float DiffValue = 0.f;
	if(player)
	{
		FVector	DynamicVector = { player->GetActorLocation() - player->GetActorForwardVector() * 30.f - player->GetActorRightVector() * 70.f + player->GetActorUpVector() * 70.f};
		if (!player->IsMoveOrJump())
		{	
			if(DiffValue == 0.f || DiffValue == -1.f)
				DiffValue = Tickvalue;
			FVector TargetVector = { DynamicVector.X, DynamicVector.Y,GetActorLocation().Z + 0.25f * UKismetMathLibrary::Sin((Tickvalue - DiffValue) * 1.5f) };
			SetActorLocation(TargetVector);
		}
		else if(player->IsMoveOrJump())
		{
			if (DiffValue != -1.f)
				DiffValue = -1.f;
			FVector TargetVector = { DynamicVector.X, DynamicVector.Y,DynamicVector.Z };
			SetActorLocation(TargetVector);
		}
		//在锁定时使机器人转向锁定的敌人
		if (player->IsLock())
		{
			//DebugNS::MyDebug(3.f, FString("Debug!"));
			FRotator DynamicRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetLockTarget()->GetActorLocation());
			SetActorRotation(DynamicRotation);
		}
		//未锁定时旋转与玩家一致
		else
		{
			SetActorRotation(player->GetActorRotation());
		}
	}
}

void AAutoRobot::OpenFire()
{
	FVector SpawnVector = { GetActorLocation() + GetActorForwardVector() * 30.f };
	GetWorld()->SpawnActor<ABulletActor>(ABulletActor::StaticClass(), SpawnVector, GetActorRotation());
}


