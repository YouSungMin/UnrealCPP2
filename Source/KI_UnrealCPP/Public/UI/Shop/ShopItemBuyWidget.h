// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemBuyWidget.generated.h"

class UImage;
class UTextBlock;
class UEditableTextBox;
class UButton;
class UOverlay;
/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UShopItemBuyWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void OnItemCountTextChange(const FText& Text);

	UFUNCTION()
	void OnItemCountTextCommited(const FText& Text, ETextCommit::Type CommitMethod);



protected:
	UPROPERTY(BlueprintReadOnly, Category = "Shop|ItemBuy", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Shop|ItemBuy", meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Shop|ItemBuy", meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemPrice = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Shop|ItemBuy", meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemStockCount = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Shop|ItemBuy", meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Shop|ItemBuy", meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ItemCount = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Shop|ItemBuy", meta = (BindWidget))
	TObjectPtr<UButton> ItemBuyButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Shop|ItemBuy", meta = (BindWidget))
	TObjectPtr<UOverlay> SoldOut = nullptr;
private:
	const int32 MinimumItemCount = 1;
};
