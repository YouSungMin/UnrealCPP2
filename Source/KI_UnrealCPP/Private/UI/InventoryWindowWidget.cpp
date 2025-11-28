// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWindowWidget.h"
#include "Player/ActionCharacter.h"
#include "Player/InventoryComponent.h"
#include "UI/InventorySlotWidget.h"

void UInventoryWindowWidget::UpdateInventoryUI(const TArray<FInvenSlot> InSlots)
{
    // 슬롯 위젯 배열 (Slot_0 ~ Slot_3)
    UInventorySlotWidget* SlotWidgets[4] = { Slot1.Get(), Slot2.Get(), Slot3.Get(), Slot4.Get() };

    for (int32 i = 0; i < 4; i++)
    {
        // 1. 위젯이 존재하고, 데이터 배열 범위 내인지 확인
        if (SlotWidgets[i] && InSlots.IsValidIndex(i))
        {
            const FInvenSlot& SlotData = InSlots[i];

            // 2. 작성하신 IsEmpty() 함수 활용
            if (!SlotData.IsEmpty() && SlotData.ItemData != nullptr)
            {
                // 아이템이 있으면: 데이터 에셋의 아이콘 + 현재 스택 수량 표시
                SlotWidgets[i]->UpdateSlot(SlotData.ItemData->ItemIcon, SlotData.GetCount());
            }
            else
            {
                SlotWidgets[i]->UpdateSlot(nullptr, 0);
            }
        }
}
