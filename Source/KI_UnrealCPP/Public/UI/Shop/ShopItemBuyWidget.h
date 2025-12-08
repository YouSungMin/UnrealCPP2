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
public:
	void SetItemData(const class UItemDataAsset* InItemData, int32 InStockCount);

	// 버튼이 활성화 비활성화를 업데이트하는 함수
	void UpdateBuyButton() const;
private:
	UFUNCTION()
	void OnItemCountTextChange(const FText& Text);

	UFUNCTION()
	void OnItemCountTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnBuyButtonClicked();

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
	// 최소 구매 갯수
	const int32 MinimumBuyCount = 1;

	// 현재 구매 가능한 갯수
	int32 StockCount = 0;

	// 현재 사려고 하는 갯수
	int32 BuyCount = MinimumBuyCount;

	// 현재 사려고하는 아이템의 데이터 에셋
	UPROPERTY()
	 TWeakObjectPtr<const UItemDataAsset> ItemData = nullptr;
};
