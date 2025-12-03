// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDetailInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UItemDetailInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetInfoIconImage(UTexture2D* InTexture);
	void SetDetailInfoText(FText InName, FText InPrice, FText InInfo);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|DetailInfo", meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|DetailInfo", meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemNameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|DetailInfo", meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemPriceText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|DetailInfo", meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemInfoText;

};
