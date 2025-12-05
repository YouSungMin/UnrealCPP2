// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemListWidget.h"
#include "UI/Shop/ShopItemBuyWidget.h"
#include "Components/VerticalBox.h"
#include "Data/DataTableRows.h"

void UShopItemListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemBuyContainer)
	{
		int count = ItemBuyContainer->GetChildrenCount();
		ShopItems.Empty(count);

		for (int32 i = 0; i < count; i++)
		{
			UShopItemBuyWidget* itemwidget = Cast< UShopItemBuyWidget>(ItemBuyContainer->GetChildAt(i));
			ShopItems.Add(itemwidget);

		}
	}
}

void UShopItemListWidget::ResetItemList(UDataTable* ItemList)
{
	float totalWight = 0.0f;

	TArray<FShopData_TableRow*> allRows;
	ItemList->GetAllRows(TEXT("ShopDataTable"),allRows);
	for (const FShopData_TableRow* row : allRows)
	{
		totalWight += row->SelectWeight;
	}
	
	int32 selectCount = ShopItems.Num();

	TArray<const FShopData_TableRow*> selectedRows;
	selectedRows.Empty(ShopItems.Num());
	for (int32 i = 0; i < selectCount; i++)
	{
		float hit = FMath::FRandRange(0.0f,totalWight);
		float currentWight = 0.0f;
		for(const FShopData_TableRow* row : allRows)
		{
			currentWight += row->SelectWeight;
			if (hit < currentWight)
			{
				selectedRows.Add(row);
				break;
			}
		}
	}

	for (int32 i = 0; i < selectCount; i++)
	{
		const FShopData_TableRow* row = selectedRows[i];
		ShopItems[i]->SetItemData(row->ItemData, row->StockCount);
	}
}
