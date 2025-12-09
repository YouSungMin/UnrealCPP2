// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHudWidget.h"
#include "Player/ActionCharacter.h"
#include "Player/ResourceComponent.h"
#include "Player/InventoryComponent.h"
#include "UI/ResourceBarWidget.h"
#include "UI/Shop/ShopWidget.h"

void UMainHudWidget::NativeConstruct()
{
	CloseInventory();
	CloseShop();
	AActionCharacter* player = Cast<AActionCharacter>(GetOwningPlayerPawn());
	if (player)
	{
		if (player && player->GetResourceComponent())
		{
			UResourceComponent* resource = player->GetResourceComponent();
			resource->OnHealthChanged.AddUObject(HealthBar.Get(), &UResourceBarWidget::RefreshWidget);
			resource->OnStaminaChanged.AddDynamic(StaminaBar.Get(), &UResourceBarWidget::RefreshWidget);		

			HealthBar->RefreshWidget(resource->GetCurrentHealth(), resource->GetMaxHealth());
			StaminaBar->RefreshWidget(resource->GetCurrentStamina(), resource->GetMaxStamina());
		}

		if (UInventoryComponent* inventoryComponent = player->GetInventoryComponent())
		{
			if (Shop)
			{
				inventoryComponent->OnInventoryMoneyChanged.AddDynamic(Shop, &UShopWidget::UpdateAllByButtonState);
			}
		}
	}
	if (Inventory && Shop)
	{
		FScriptDelegate sellDelegate;
		sellDelegate.BindUFunction(Inventory, "CloseDetailInfo");
		Shop->AddToItemSellDelegate(sellDelegate);
	}
}

void UMainHudWidget::OpenInventory()
{
	Inventory->RefreshInventoryWidget();
	Inventory->SetVisibility(ESlateVisibility::Visible);
	InventoryState = EOpenState::Open;
}

void UMainHudWidget::CloseInventory()
{
	InventoryState = EOpenState::Close;
	Inventory->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHudWidget::OpenShop(UDataTable* ItemList)
{
	if (ItemList)
	{
		Shop->InitailzeShop(ItemList);
		Shop->SetVisibility(ESlateVisibility::Visible);
		ShopState = EOpenState::Open;
	}
}

void UMainHudWidget::CloseShop()
{
	ShopState = EOpenState::Close;
	Shop->SetVisibility(ESlateVisibility::Hidden);
}
