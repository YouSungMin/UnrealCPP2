// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemporarySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UTemporarySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetItemIconImage(UTexture2D* InTexture);
	void SetCountText(int32 InCount);

protected:
	// 아이템 소지 갯수 텍스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UTextBlock> CountText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage;
};
