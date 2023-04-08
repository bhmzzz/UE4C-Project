// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSkillActor.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Particles/ParticleSystem.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "Sound/SoundCue.h"

// Sets default values
ATestSkillActor::ATestSkillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Skillbox = CreateDefaultSubobject<UBoxComponent>("SkillMesh");
	Skillbox->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ATestSkillActor::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	////延迟0.5s后调用播放居合斩特效的函数
	//GetWorldTimerManager().SetTimer(handler1, this, &ATestSkillActor::CallFunction, 1.25f, false);
	PlayParticlesEffect();
}

// Called every frame
void ATestSkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestSkillActor::PlayParticlesEffect()
{

	//GetWorldTimerManager().ClearTimer(handler1);
	//居合特效
	ANormalEnemyCharacter* Target = Cast<ANormalEnemyCharacter>(Player->GetLockTarget());
	UWorld* world = GetWorld();
	UParticleSystem* ParticleTemplate1 = LoadObject<UParticleSystem>(NULL, TEXT("ParticleSystem1'/Game/TrailPack/Particles/Cascade/Speed/Hit/P_SpeedHit.P_SpeedHit'"));
	UParticleSystem* ParticleTemplate2 = LoadObject<UParticleSystem>(NULL, TEXT("ParticleSystem2'/Game/TrailPack/Particles/Cascade/Magic/Hit/P_MagicHitSoft.P_MagicHitSoft'"));
	USoundCue* CYZSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Magic/wav/CYZSound.CYZSound"));

	if (ParticleTemplate1 && Target && ParticleTemplate2 && CYZSound)
	{
		UParticleSystemComponent* ParticleComponent1 = UGameplayStatics::SpawnEmitterAtLocation(world, ParticleTemplate1, GetActorTransform(), true);
		ParticleComponent1->SetAutoAttachmentParameters(GetRootComponent(), FName(), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget);
		UParticleSystemComponent* ParticleComponent2 = UGameplayStatics::SpawnEmitterAtLocation(world, ParticleTemplate2, GetActorTransform(), true);
		ParticleComponent2->SetAutoAttachmentParameters(GetRootComponent(), FName(), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget);
		UGameplayStatics::PlaySound2D(GetWorld(), CYZSound);
		Target->SetHittedValue(true);
		Target->TakeDamageAndAmmor(5.f);
	}
	GetWorldTimerManager().SetTimer(handler1, this, &ATestSkillActor::MyDestroyActor, 0.5f, false);

}

void ATestSkillActor::CallFunction()
{
	PlayParticlesEffect();
}

void ATestSkillActor::MyDestroyActor()
{
	GetWorldTimerManager().ClearTimer(handler1);
	Destroy();
}


