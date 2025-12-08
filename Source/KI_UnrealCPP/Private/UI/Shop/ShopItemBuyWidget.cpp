// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemBuyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Data/ItemDataAsset.h"
#include "Data/DataTableRows.h"
#include "Player/InventoryOwner.h"

void UShopItemBuyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemCount)
	{
		ItemCount->SetHintText(FText::AsNumber(MinimumBuyCount));
		ItemCount->OnTextChanged.AddDynamic(this, &UShopItemBuyWidget::OnItemCountTextChange);	// 변경이 있을 때
		ItemCount->OnTextCommitted.AddDynamic(this, &UShopItemBuyWidget::OnItemCountTextCommited);	// 변경을 확정했을 때(엔터 입력 후 , 포커스를 잃은 후)

	}
	if (ItemBuyButton)
	{
		ItemBuyButton->OnClicked.AddDynamic(this, &UShopItemBuyWidget::OnBuyButtonClicked);
	}

}

void UShopItemBuyWidget::SetItemData(const UItemDataAsset* InItemData, int32 InStockCount)
{
	ItemIcon->SetBrushFromTexture(InItemData->ItemIcon);
	ItemName->SetText(InItemData->ItemName);
	ItemPrice->SetText(FText::AsNumber(InItemData->ItemPrice));
	ItemStockCount->SetText(FText::AsNumber(InStockCount));
	ItemDescription->SetText(InItemData->ItemDescription);

	BuyCount = MinimumBuyCount;
	StockCount = InStockCount;
	ItemData = InItemData;

	UpdateBuyButton();
}

void UShopItemBuyWidget::OnItemCountTextChange(const FText& Text)
{
	UE_LOG(LogTemp,Log,TEXT("Changed : %s"), *Text.ToString());

	FString number = Text.ToString();
	if (number.IsNumeric())
	{
		BuyCount = FMath::Clamp(FCString::Atoi(*number),MinimumBuyCount,StockCount);
		ItemCount->SetText(FText::AsNumber(BuyCount));

		UpdateBuyButton();
	}
}

void UShopItemBuyWidget::OnItemCountTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	UE_LOG(LogTemp, Log, TEXT("Commited : %s"), *Text.ToString());

	FString number = Text.ToString();
	if (number.IsNumeric())
	{
		int32 count = FCString::Atoi(*number);
		ItemCount->SetText(FText::AsNumber(count));
	}
	else
	{
		ItemCount->SetText(FText::AsNumber(MinimumBuyCount));
	}
}

void UShopItemBuyWidget::OnBuyButtonClicked()
{

	//FString CountStr = ItemCount->GetText().ToString();
	//if (CountStr.IsNumeric())
	//{
	//	int32 BuyCount = FCString::Atoi(*CountStr);

	//	if (BuyCount > 0 || BuyCount <= StockCount)
	//	{
	//		if (OwningPawn->Implements<UInventoryOwner>())
	//		{
	//			// 현재 소지금 확인
	//			int32 CurrentGold = IInventoryOwner::Execute_GetCurrentMoney(OwningPawn.Get());
	//			int32 TotalPrice = BuyCount * ItemData->ItemPrice;
	//			if (CurrentGold >= TotalPrice)
	//			{
	//				IInventoryOwner::Execute_RemoveMoney(OwningPawn.Get(),TotalPrice);
	//				//IInventoryOwner::Execute_AddItem(OwningPawn.Get(),ShopItemData.Get(), BuyCount);
	//			}
	//		}
	//	}
	//}
}

void UShopItemBuyWidget::UpdateBuyButton() const
{
	APawn* player = GetOwningPlayerPawn();
	if (player->Implements<UInventoryOwner>())
	{
		bool hasEnoughMoney = IInventoryOwner::Execute_HasEnoughMoney(player, BuyCount * ItemData->ItemPrice);

		ItemBuyButton->SetIsEnabled(hasEnoughMoney);
	}
}
