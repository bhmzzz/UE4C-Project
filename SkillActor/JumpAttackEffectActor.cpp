// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpAttackEffectActor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AJumpAttackEffectActor::AJumpAttackEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJumpAttackEffectActor::BeginPlay()
{
	Super::BeginPlay();
	PlayParticlesEffect();
}

void AJumpAttackEffectActor::PlayParticlesEffect()
{
	//ÌøÔ¾¹¥»÷ÌØÐ§
	UWorld* world = GetWorld();
	UParticleSystem* ParticleTemplate1 = LoadObject<UParticleSystem>(NULL, TEXT("ParticleSystem1'/Game/TrailPack/Particles/Cascade/Magic/Hit/P_MagicHitFinisher.P_MagicHitFinisher'"));
	UParticleSystem* ParticleTemplate2 = LoadObject<UParticleSystem>(NULL, TEXT("ParticleSystem2'/Game/TrailPack/Particles/Cascade/Magic/Hit/P_MagicHitSoft.P_MagicHitSoft'"));
	if (ParticleTemplate1 && ParticleTemplate2)
	{
		UParticleSystemComponent* ParticleComponent1 = UGameplayStatics::SpawnEmitterAtLocation(world, ParticleTemplate1, GetActorTransform(), true);
		//ParticleComponent1->SetAutoAttachmentParameters(GetRootComponent(), FName(), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget);
		UParticleSystemComponent* ParticleComponent2 = UGameplayStatics::SpawnEmitterAtLocation(world, ParticleTemplate2, GetActorTransform(), true);
		//ParticleComponent2->SetAutoAttachmentParameters(GetRootComponent(), FName(), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget);
	}
	GetWorldTimerManager().SetTimer(handler1, this, &AJumpAttackEffectActor::MyDestroyActor, 0.5f, false);
}

void AJumpAttackEffectActor::CallFunction()
{
	PlayParticlesEffect();
}

void AJumpAttackEffectActor::MyDestroyActor()
{
	GetWorldTimerManager().ClearTimer(handler1);
	Destroy();
}

