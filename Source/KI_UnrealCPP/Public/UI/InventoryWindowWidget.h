// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWindowWidget.generated.h"

/**
 * 
 */
 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIventoryCloseRequested);

UCLASS()
class KI_UNREALCPP_API UInventoryWindowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct()override;

	UPROPERTY(BlueprintAssignable, Category = "UI|Inventory");
	FOnIventoryCloseRequested OnInventoryCloseRequested;
	//void UpdateInventoryUI(const TArray<struct FInvenSlot> InSlots);

private:
	UFUNCTION()
	void OnCloseClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CloseButton = nullptr;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	//TWeakObjectPtr<class UInventorySlotWidget> Slot1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	//TWeakObjectPtr<class UInventorySlotWidget> Slot2;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	//TWeakObjectPtr<class UInventorySlotWidget> Slot3;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	//TWeakObjectPtr<class UInventorySlotWidget> Slot4;
};
