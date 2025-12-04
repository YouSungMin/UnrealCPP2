// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHudWidget.h"
#include "Player/ActionCharacter.h"
#include "Player/ResourceComponent.h"
#include "UI/ResourceBarWidget.h"
#include "UI/Shop/ShopWidget.h"

void UMainHudWidget::NativeConstruct()
{
	CloseInventory();
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
		if (UInventoryComponent* inventoryComponent  = player->GetInventoryComponent())
		{
			//Inventory->OnInventoryCloseRequested.AddDynamic(this, &UMainHudWidget::CloseInventory);

			// inventoryComponent의 내용을 바탕으로 InventoryWidget을 채우기
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
	OpenState = EOpenState::Open;
}

void UMainHudWidget::CloseInventory()
{
	OpenState = EOpenState::Close;
	Inventory->SetVisibility(ESlateVisibility::Hidden);
}
