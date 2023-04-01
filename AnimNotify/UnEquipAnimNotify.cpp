// Fill out your copyright notice in the Description page of Project Settings.


#include "UnEquipAnimNotify.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"

UUnEquipAnimNotify::UUnEquipAnimNotify()
{
}

void UUnEquipAnimNotify::Notify(USkeletalMeshComponent* MenshComp, UAnimSequenceBase* AnimSeq)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(MenshComp->GetOwner());
	if (Player)
	{
		UStaticMeshComponent* WeaponMesh = Player->GetWeapon();
		if (WeaponMesh)
		{
			WeaponMesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,FName("EquipPlace"));
		}
	}
}
