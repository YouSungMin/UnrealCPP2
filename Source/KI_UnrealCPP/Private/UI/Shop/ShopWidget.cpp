// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopWidget.h"
#include "UI/Shop/ShopItemSellWidget.h"
#include "UI/Shop/ShopItemListWidget.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ResetProductListWidget();
}

void UShopWidget::AddToItemSellDelegate(const FScriptDelegate& Delegate)
{
	ItemSellWidget->OnItemSell.Add(Delegate);
}

void UShopWidget::UpdateAllByButtonState(int32 _)
{
	 if(GetVisibility() == ESlateVisibility::Visible || 
	 GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	 {
		 ItemListWidget->UpdateAllBuyButton();
	 }
}

void UShopWidget::OnShopCloseClicked()
{
}

void UShopWidget::ResetProductListWidget()
{
	if (ShopItemList.IsValid())
	{
		ItemListWidget->ResetItemList(ShopItemList.Get());
	}
}
