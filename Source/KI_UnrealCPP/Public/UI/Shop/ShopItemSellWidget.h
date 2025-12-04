// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemSellWidget.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UShopItemSellWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitializeInventoryWidget(class UInventoryComponent* InventoryComponent);

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

private:
	TWeakObjectPtr<class UInventoryComponent> TargetInventory = nullptr;
};
