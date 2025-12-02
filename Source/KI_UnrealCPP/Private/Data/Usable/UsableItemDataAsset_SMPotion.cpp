// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Usable/UsableItemDataAsset_SMPotion.h"
#include "Player/HasStamina.h"

void UUsableItemDataAsset_SMPotion::UseItem_Implementation(AActor* InTarget)
{
	if (InTarget->Implements<UHasStamina>())
	{
		IHasStamina::Execute_RecoveryStamina(InTarget,RecoveryAmount);
	}
}
