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

	if (SoldOut)
	{
		SoldOut->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UShopItemBuyWidget::SetItemData(UItemDataAsset* InItemData, int32 InStockCount)
{
	ItemCount->SetIsEnabled(true);
	SoldOut->SetVisibility(ESlateVisibility::Hidden);

	ItemIcon->SetBrushFromTexture(InItemData->ItemIcon);
	ItemName->SetText(InItemData->ItemName);
	ItemPrice->SetText(FText::AsNumber(InItemData->ItemPrice));
	ItemDescription->SetText(InItemData->ItemDescription);

	//ItemStockCount->SetText(FText::AsNumber(InStockCount));
	//StockCount = InStockCount;
	ItemData = InItemData;
	SetStockCount(InStockCount);
	SetBuyCount(MinimumBuyCount);

	UpdateBuyButton();
}

void UShopItemBuyWidget::OnItemCountTextChange(const FText& Text)
{
	UE_LOG(LogTemp,Log,TEXT("Changed : %s"), *Text.ToString());

	FString number = Text.ToString();
	if (number.IsNumeric())
	{
		SetBuyCount(FMath::Clamp(FCString::Atoi(*number), MinimumBuyCount, StockCount));

		UpdateBuyButton();
	}
}

void UShopItemBuyWidget::OnItemCountTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	UE_LOG(LogTemp, Log, TEXT("Commited : %s"), *Text.ToString());

	FString number = Text.ToString();
	if (number.IsNumeric())
	{
		SetBuyCount(FCString::Atoi(*number));
	}
	else
	{
		SetBuyCount(MinimumBuyCount);
	}
}

void UShopItemBuyWidget::OnBuyButtonClicked()
{
	APawn* player = GetOwningPlayerPawn();
	if (player->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddItem(player,ItemData.Get(), BuyCount);
		IInventoryOwner::Execute_RemoveMoney(player,ItemData->ItemPrice * BuyCount);
		SetStockCount(StockCount - BuyCount);
		SetBuyCount(MinimumBuyCount);
		if (StockCount < MinimumBuyCount)
		{
			ItemCount->SetIsEnabled(false);
			SoldOut->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UShopItemBuyWidget::SetStockCount(int32 InCount)
{
	StockCount = InCount;
	ItemStockCount->SetText(FText::AsNumber(StockCount));
	UpdateBuyButton();
}

void UShopItemBuyWidget::SetBuyCount(int32 InCount)
{
	BuyCount = InCount;
	if (BuyCount > MinimumBuyCount)
	{
		ItemCount->SetText(FText::AsNumber(BuyCount));
	}
	else
	{
		ItemCount->SetText(FText::GetEmpty());
	}
}

void UShopItemBuyWidget::UpdateBuyButton() const
{
	if (StockCount < MinimumBuyCount)
	{
		ItemBuyButton->SetIsEnabled(false);
	}
	else
	{
		APawn* player = GetOwningPlayerPawn();
		if (player->Implements<UInventoryOwner>())
		{
			bool hasEnoughMoney = IInventoryOwner::Execute_HasEnoughMoney(player, BuyCount * ItemData->ItemPrice);

			ItemBuyButton->SetIsEnabled(hasEnoughMoney);
		}
	}
}
