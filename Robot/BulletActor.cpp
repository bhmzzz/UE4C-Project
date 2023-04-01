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
	BulletColl->SetEnableGravity(false);	//�ر��ӵ�����

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetRelativeLocation(FVector(0.f, 0.f, -45.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletMeshAssert(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (BulletMeshAssert.Succeeded())
	{
		BulletMesh->SetStaticMesh(BulletMeshAssert.Object);	//�ڱ༭�����ҵ���Ӧ��StaticMesh�����RobotMesh��
		SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	}
	BulletMesh->SetEnableGravity(false);	//�ر��ӵ�����
	
	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->InitialSpeed = 3000.f;	//�����ӵ���ʼ�ٶ�
	ProjectileComp->MaxSpeed = 3000.f;		//�����ӵ�����ٶ�
	ProjectileComp->ProjectileGravityScale = 0.f;	//�رշ������������

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
	//�����������ٶ�Ϊ0ʱDestory�ö������ڷ�ֹ�ӵ��ٶȽ�Ϊ0��Ȼ���ڵ������
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
