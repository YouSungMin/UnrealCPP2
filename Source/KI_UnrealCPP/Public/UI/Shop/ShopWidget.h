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
public:

private:
	UFUNCTION()
	void OnShopCloseClicked();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UShopItemSellWidget> ShopItemSell = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> CloseButton = nullptr;
};
