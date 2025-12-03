// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWindowWidget.h"
#include "UI/InventorySlotWidget.h"
#include "UI/InventorySlotWidget.h"
#include "UI/GoldPanelWidget.h"
#include "UI/InventoryDragDropOperation.h"
#include "Player/InventoryComponent.h"
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
            TargetInventory->OnInventoryMoneyChanged.BindUFunction(this, "RefreshMoneyPanel");
            TargetInventory->OnInventorySlotChanged.BindUFunction(this, "RefreshSlotWidget");
            RefreshMoneyPanel(0);

            int32 size = FMath::Min(SlotGridPanel->GetChildrenCount(), TargetInventory->GetInventorySize());
            SlotWidgets.Empty(size);
            for (int i = 0; i < TargetInventory->GetInventorySize(); i++)
            {
                UInventorySlotWidget* slotWidget = Cast<UInventorySlotWidget>(SlotGridPanel->GetChildAt(i));
                slotWidget->InitializeSlot(TargetInventory.Get(), i);

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

void UInventoryWindowWidget::RefreshSlotWidget(int32 InSlotIndex)
{
    if (IsValidIndex(InSlotIndex))
    {
        SlotWidgets[InSlotIndex]->RefreshSlot();
    }
}

void UInventoryWindowWidget::RefreshMoneyPanel(int32 CurrentMoney)
{
    GoldPanel->SetGold(CurrentMoney);
}

void UInventoryWindowWidget::ClearInventoryWidget()
{
    TargetInventory = nullptr;
}

bool UInventoryWindowWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UInventoryDragDropOperation* invenOp = Cast<UInventoryDragDropOperation>(InOperation);

    if (invenOp)
    {
        UE_LOG(LogTemp,Log,TEXT("인벤토리에 드랍 : 원래 슬롯으로 아이템이 돌아가야 한다"));
        TargetInventory->SetItemAtIndex(invenOp->StartIndex, invenOp->ItemData.Get(), invenOp->Count);

        return true;

     }
    return false;
}

void UInventoryWindowWidget::OnCloseClicked()
{
    OnInventoryCloseRequested.Broadcast();
}
