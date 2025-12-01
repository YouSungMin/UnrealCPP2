// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWindowWidget.h"
#include "UI/InventorySlotWidget.h"
#include "Player/InventoryComponent.h"
#include "UI/InventorySlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"

void UInventoryWindowWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UInventoryWindowWidget::OnCloseClicked);
    }

}

void UInventoryWindowWidget::InitializeInventoryWidget(UInventoryComponent* InventoryComponent)
{
    if (InventoryComponent)
    {
        TargetInventory = InventoryComponent;
        if (TargetInventory.IsValid())
        {
            UE_LOG(LogTemp,Log,TEXT("인벤토리 위젯 초기화"));

            if (SlotGridPanel->GetChildrenCount() != TargetInventory->GetInventorySize())
            {
                UE_LOG(LogTemp,Log,TEXT("인벤토리 컴포넌트와 위젯의 슬롯 크기가 다릅니다."));  
                return;
            }

            int32 size = FMath::Min(SlotGridPanel->GetChildrenCount(), TargetInventory->GetInventorySize());
            SlotWidgets.Empty(size);
            for (int i = 0; i < TargetInventory->GetInventorySize(); i++)
            {
                FInvenSlot* slotData = TargetInventory->GetSlotData(i);
                UInventorySlotWidget* slotWidget = Cast<UInventorySlotWidget>(SlotGridPanel->GetChildAt(i));
                slotWidget->InitializeSlot(i, slotData);
                SlotWidgets.Add(slotWidget);
            }
        }
    }
}

void UInventoryWindowWidget::RefreshInventoryWidget()
{ 
    for(const UInventorySlotWidget* slot : SlotWidgets)
    {
        slot->RefreshSlot();
    }

}

void UInventoryWindowWidget::ClearInventoryWidget()
{
    TargetInventory = nullptr;
}

void UInventoryWindowWidget::OnCloseClicked()
{
    OnInventoryCloseRequested.Broadcast();
}
