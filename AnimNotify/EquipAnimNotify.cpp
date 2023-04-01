// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipAnimNotify.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"

UEquipAnimNotify::UEquipAnimNotify()
{
}

void UEquipAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp,Animation);
	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player)
	{
		UStaticMeshComponent* WeaponMesh = Player->GetWeapon();
		if (WeaponMesh)
			WeaponMesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,FName("RHandWeapon"));
		else
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Can't get weapon normal!"));
	}
}


