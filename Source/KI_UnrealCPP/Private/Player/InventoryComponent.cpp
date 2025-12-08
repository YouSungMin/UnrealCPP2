// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InventoryComponent.h"
#include "Data/Usable/UsableItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Slots.SetNum(InventorySize);
}

void UInventoryComponent::AddMoney(int32 InIncome)
{
	Money += InIncome;
	OnInventoryMoneyChanged.Broadcast(Money);
}

int32 UInventoryComponent::AddItem(UItemDataAsset* InItemData, int32 InCount)
{

	int32 remainingCount = InCount;
	if (InItemData && InCount > 0) // 추가가 가능할 때만 추가
	{
		// 같은 종류의 아이템이 들어있는 슬롯을 찾아 추가
		int32 startIndex = 0;
		while (remainingCount > 0)	//remainingCount가 남아있는한 반복(같은 종류의 아이템이 있는 슬롯이 없으면 break;)
		{
			int32 foundIndex = FindSlotWithItem(InItemData,startIndex); // 같은 종류의 아이템이 들어있고 공간에 여유가 있는 슬롯 찾기 시도
			if(foundIndex == UInventoryComponent::InventoryFail)
				break;	// 같은 종류의 아이템이 들어있는 슬롯이 없으면 "While (remaingCount > 0)"를 break;

			// 같은 종류의 아이템이 들어있는 슬롯을 찾았다.
			FInvenSlot& slot = Slots[foundIndex];
			int32 availableCount = slot.ItemData->ItemMaxStackCount - slot.GetCount(); // 슬롯에 추가 가능한 갯수가 몇개인지 확인
			if (availableCount > 0)	//  추가가 가능하면
			{
				int32 amountToAdd = FMath::Min(availableCount, remainingCount);		// 추가량 결정
				SetItemAtIndex(foundIndex,InItemData,slot.GetCount()+ amountToAdd);	// 결정된 추가량 만큼 추가
				remainingCount -= amountToAdd; //remainingCount를 슬롯에 추가한 만큼 감소
			}
			startIndex = foundIndex +1; //FindSlotWithItem에서 현재 슬롯 다음부터 찾게 하기
		}

		while (remainingCount > 0)	//remainingCount가 남아있는한 반복(빈슬롯이 없으면 break)
		{
			int32 emptyIndex = FindEmptySlot();
			if(emptyIndex == UInventoryComponent::InventoryFail)
				break; // 빈슬롯이 없다.

			int32 amountToAdd = FMath::Min(InItemData->ItemMaxStackCount, remainingCount);	// 추가량 결정
			SetItemAtIndex(emptyIndex, InItemData, amountToAdd);	// 결정된 추가량 만큼 추가
			remainingCount -= amountToAdd; //remainingCount를 슬롯에 추가한 만큼 감소
		}

		// 같은 종류의 아이템이 들어있는 슬롯과 빈슬롯이 모두 채우고도 남은 아이템이 있다. => remainingCount 리턴
	}
	return remainingCount;
}

void UInventoryComponent::UseItem(int32 InUseIndex)
{
	UE_LOG(LogTemp,Log,TEXT("Inven %d Slot : 사용됨"),InUseIndex);
	FInvenSlot* slot = GetSlotData(InUseIndex);
	
	if (slot->ItemData && slot->ItemData->Implements<UUsableItem>())
	{
		IUsableItem::Execute_UseItem(slot->ItemData, this->GetOwner());

		UpdateSlotCount(InUseIndex, -1);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Inven %d Slot : 비어있거나 사용할 수 없는 아이템"), InUseIndex);
	}
}

void UInventoryComponent::SetItemAtIndex(int32 InSlotIndex, UItemDataAsset* InItemData, int32 InCount)
{
	if (IsValidIndex(InSlotIndex))
	{
		FInvenSlot& TargetSlot = Slots[InSlotIndex];

		TargetSlot.ItemData = InItemData;
		TargetSlot.SetCount(InCount);		//InCount가 0이면 자동 Clear

		OnInventorySlotChanged.ExecuteIfBound(InSlotIndex);
	}
}

void UInventoryComponent::UpdateSlotCount(int32 InSlotIndex, int32 InDeltaCount)
{
	if (IsValidIndex(InSlotIndex))
	{
		FInvenSlot& TargetSlot = Slots[InSlotIndex];

		if (TargetSlot.IsEmpty()) return;	// 슬롯이 비어있으면 변화할 수 없음

		int32 NewCount = TargetSlot.GetCount() + InDeltaCount;
		SetItemAtIndex(InSlotIndex, TargetSlot.ItemData, NewCount);
	}
}

void UInventoryComponent::ClearSlotAtIndex(int32 InSlotIndex)
{
	//if (IsValidIndex(InSlotIndex))
	//{
	//	FInvenSlot& TargetSlot = Slots[InSlotIndex];
	//	TargetSlot.Clear();
	//	OnInventorySlotChanged.ExecuteIfBound(InSlotIndex);
	//}
	SetItemAtIndex(InSlotIndex, nullptr, 0);
}

FInvenSlot* UInventoryComponent::GetSlotData(int32 InSlotIndex)
{
	check(IsValidIndex(InSlotIndex));

	
	return &Slots[InSlotIndex];
}

int32 UInventoryComponent::FindSlotWithItem(UItemDataAsset* InItemData, int32 InStartIndex)
{
	int32 result = UInventoryComponent::InventoryFail;
	int32 size = Slots.Num();

	for (int32 i = InStartIndex; i < size; i++)
	{
		if (Slots[i].ItemData == InItemData && !Slots[i].IsFull())
		{
			result = i;
			break;
		}
	}
	return result;
}

int32 UInventoryComponent::FindEmptySlot()
{
	int32 result = UInventoryComponent::InventoryFail;
	int32 size = Slots.Num();

	for (int32 i = 0; i < size; i++)
	{
		if (Slots[i].IsEmpty())
		{
			result = i;
			break;
		}
	}
	return result;
}


