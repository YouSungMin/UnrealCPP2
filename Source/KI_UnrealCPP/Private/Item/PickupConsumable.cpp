// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickupConsumable.h"
#include "Player/InventoryOwner.h"

void APickupConsumable::OnPickupComplete_Implementation()
{
	OnConsume();

	Super::OnPickupComplete_Implementation(); // 자기 자신을 삭제시키기 때문에 마지막에 처리
}

void APickupConsumable::OnConsume()
{
	if (DataAsset)
	{
		DataAsset->Excute(PickupOwner.Get());
	}
}