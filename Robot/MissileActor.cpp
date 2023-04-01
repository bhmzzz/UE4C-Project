// Fill out your copyright notice in the Description page of Project Settings.


#include "MissileActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "Components/SphereComponent.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"

// Sets default values
AMissileActor::AMissileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//sphereColl->OnComponentBeginOverlap.AddDynamic(this, &AMissileActor::TargetOverlap);
	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>("MissileMesh");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MissileMeshAssert(TEXT("/Game/modle/MissileMesh"));
	if (MissileMeshAssert.Succeeded())
	{
		MissileMesh->SetStaticMesh(MissileMeshAssert.Object);	//在编辑器中找到对应的StaticMesh分配给RobotMesh。
		//SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	}
	RootComponent = MissileMesh;
}

// Called when the game starts or when spawned
void AMissileActor::BeginPlay()
{
	Super::BeginPlay();
	InitSocketLocation();
	SamplePoint = GetBezierCurveSamplePoint();
}

TArray<FVector> AMissileActor::GetBezierCurveSamplePoint()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	TArray<FVector> Result;
	if (player)
	{
		if (player->GetLockTarget() != nullptr)
		{
			FVector TargetLocation = player->GetLockTarget()->GetActorLocation();
			FVector p0 = GetActorLocation();
			FVector p3 = TargetLocation;
			FVector dif = p3 - p0;
			float xy_value = sqrt(pow(dif.X, 2) + pow(dif.Y, 2));
			FVector p1 = { p0.X,p0.Y,p0.Z + xy_value / 3.f };
			FVector p2 = { p0.X + dif.X / 2.f,p0.Y + dif.Y / 2.f,p0.Z + xy_value / 4.f };
			if (MissileCount::count <= 3)
			{
				if (MissileCount::count == 3)
				{
					MissileCount::count = 0;
				}
				MissileCount::count++;
				count = MissileCount::count;
			}
			switch (count)
			{
			case 1:
			{
				p1 = { p0.X,p0.Y,p0.Z + xy_value / 3.f };
				p2 = { p0.X + dif.X / 2.f,p0.Y + dif.Y / 2.f,p0.Z + xy_value / 4.f };
				break;
			}
			case 2:
			{
				p1 = FVector(p0.X, p0.Y, p0.Z) + (-GetActorRightVector() * xy_value / 3.f);
				p2 = FVector( p0.X + dif.X / 2.f,p0.Y + dif.Y / 2.f,p0.Z) + (-GetActorRightVector() * xy_value / 4.f);
				break;
			}
			case 3:
			{
				p1 = FVector(p0.X, p0.Y, p0.Z) + (GetActorRightVector() * xy_value / 3.f);
				p2 = FVector(p0.X + dif.X / 2.f, p0.Y + dif.Y / 2.f, p0.Z) + (GetActorRightVector() * xy_value / 4.f);
				break;
			}
			default:
			{
				break;
			}
			}
			Result.Push(p0);
			Result.Push(p1);
			Result.Push(p2);
			Result.Push(p3);
		}
	}
	return Result;
}

FVector AMissileActor::BezierCurvePath(TArray<FVector> SamplePoints, float alpha)
{
	if (SamplePoints.Num() == 0)
		return FVector{};
	TArray<FVector> NextPoints;
	if (SamplePoints.Num() == 1)
		return SamplePoints[0];
	for (int i = 0; i < SamplePoints.Num() - 1; ++i)
		NextPoints.Push(UKismetMathLibrary::VLerp(SamplePoints[i], SamplePoints[i + 1], alpha));
	return BezierCurvePath(NextPoints,alpha);
}

void AMissileActor::DestroyThisActor()
{
	FindMissileFlyCollision();
	if (SamplePoint.Num() == 4)
	{
		if (GetActorLocation().Equals(SamplePoint[3], 0.01))
		{
			MissileExplosion();
			Destroy();
		}
	}
}

void AMissileActor::MissileExplosion()
{
	UParticleSystem* particalSystem = LoadObject<UParticleSystem>(NULL, TEXT("particleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	USoundBase* ExplosionSound2D = LoadObject<USoundBase>(NULL, TEXT("ExplosionSound2D'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	if (particalSystem && ExplosionSound2D)
	{
		FTransform ActorTransform = { GetActorTransform().GetRotation(),GetActorTransform().GetTranslation(),FVector(2.f,2.f,2.f) };
		UParticleSystemComponent* ExplosionComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particalSystem, ActorTransform,true);
		UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound2D);
	}
}

void AMissileActor::FindMissileFlyCollision()
{
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes = { UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1),UEngineTypes::ConvertToObjectType(ECC_EngineTraceChannel1),
	UEngineTypes::ConvertToObjectType(ECC_EngineTraceChannel1) };
	TArray<AActor*> ActorToIgnore = {UGameplayStatics::GetPlayerCharacter(GetWorld(),0)};
	TArray<FHitResult> HitResult;
	//寻找导弹在到达目的地前遇上的碰撞物
	if (std::abs(CurrentLocation.Size() - MissileMesh->GetSocketLocation(FName("Socket0")).Size()) < 1)
	{
		return;
	}
	else
	{
		if (UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), CurrentLocation, MissileMesh->GetSocketLocation(FName("Socket0")),
			ObjectTypes, false, ActorToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
		{
			HasHitEnemy = true;
			for (auto& value : HitResult)
			{
				ANormalEnemyCharacter* Enemy = Cast<ANormalEnemyCharacter>(value.Actor);
				if (Enemy)
				{
					MissileExplosion();
					break;
				}
			}
		}
		InitSocketLocation();
	}
}

void AMissileActor::InitSocketLocation()
{
	CurrentLocation = MissileMesh->GetSocketLocation(FName("Socket0"));
}

// Called every frame
void AMissileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DestroyThisActor();

}

