// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopCloseRequested);
/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	void InitailzeShop(UDataTable* ItemList);

	void AddToItemSellDelegate(const FScriptDelegate& Delegate);
	void AddToShopCloseDelegate(const FScriptDelegate& Delegate);

	UFUNCTION()
	void UpdateAllByButtonState(int32 _);

private:
	UFUNCTION()
	void OnShopCloseClicked();
protected:
	void ResetProductListWidget();
public:
	FOnShopCloseRequested OnShopCloseRequested;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Shop|Sell", meta = (BindWidget))
	TObjectPtr<class UShopItemSellWidget> ItemSellWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Shop|Buy", meta = (BindWidget))
	TObjectPtr<class UShopItemListWidget> ItemListWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> CloseButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Buy")
	TWeakObjectPtr<UDataTable> ShopItemList = nullptr;
};
