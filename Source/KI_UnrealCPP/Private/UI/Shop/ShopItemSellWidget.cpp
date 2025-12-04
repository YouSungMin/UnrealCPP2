// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemSellWidget.h"
#include "UI/InventoryDragDropOperation.h"
#include "Player/InventoryOwner.h"

bool UShopItemSellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UInventoryDragDropOperation* invenOp = Cast<UInventoryDragDropOperation>(InOperation);

    if (invenOp)
    {
        //UE_LOG(LogTemp,Log,TEXT("Shop Item Sell Drop : %d Slot , Item : %s"), invenOp->StartIndex, *(invenOp->ItemData->ItemName.ToString()));
        if (invenOp->ItemData.IsValid())
        {
            int32 sellPrice = invenOp->Count * invenOp->ItemData->ItemPrice * 0.5f;
            APawn* player = GetOwningPlayerPawn();
            if (player->Implements<UInventoryOwner>())
            {
                IInventoryOwner::Execute_AddMoney(player, sellPrice);
                OnItemSell.Broadcast();
                return true;
            }
        }

        return true;
    }

    return false;
}
