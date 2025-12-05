// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

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
	void AddToItemSellDelegate(const FScriptDelegate& Delegate);

private:
	UFUNCTION()
	void OnShopCloseClicked();
protected:
	void ResetProductListWidget();

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
