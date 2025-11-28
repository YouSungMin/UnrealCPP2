// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWindowWidget.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UInventoryWindowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateInventoryUI(const TArray<class FInvenSlot> InSlots);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TWeakObjectPtr<class UInventorySlotWidget> Slot1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TWeakObjectPtr<class UInventorySlotWidget> Slot2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TWeakObjectPtr<class UInventorySlotWidget> Slot3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TWeakObjectPtr<class UInventorySlotWidget> Slot4;
};
