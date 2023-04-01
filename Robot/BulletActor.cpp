// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletColl = CreateDefaultSubobject<USphereComponent>(TEXT("BulletColl"));
	BulletColl->SetSphereRadius(60.f);
	RootComponent = BulletColl;
	BulletColl->SetEnableGravity(false);	//关闭子弹重力

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetRelativeLocation(FVector(0.f, 0.f, -45.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletMeshAssert(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (BulletMeshAssert.Succeeded())
	{
		BulletMesh->SetStaticMesh(BulletMeshAssert.Object);	//在编辑器中找到对应的StaticMesh分配给RobotMesh。
		SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	}
	BulletMesh->SetEnableGravity(false);	//关闭子弹重力
	
	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->InitialSpeed = 3000.f;	//设置子弹初始速度
	ProjectileComp->MaxSpeed = 3000.f;		//设置子弹最大速度
	ProjectileComp->ProjectileGravityScale = 0.f;	//关闭发射物组件重力

	SetActorScale3D(FVector(0.1f, 0.1f, 0.1f));
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//当该类对象的速度为0时Destory该对象，用于防止子弹速度降为0仍然存在的情况。
	if (GetVelocity() == FVector{ 0.f,0.f,0.f })
	{
		Destroy();
	}
	DestroyWhenTooFar();
}

void ABulletActor::DestroyWhenTooFar()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	float dis = std::abs((GetActorLocation() - Player->GetActorLocation()).Size());
	if (dis > 5000)
		Destroy();
}
