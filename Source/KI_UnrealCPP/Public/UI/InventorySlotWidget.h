// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 슬롯을 업데이트 하는 함수
	void UpdateSlot(class UTexture2D* IconTexture, int32 InItemCount);
protected:
	// 아이템 갯수 텍스트
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	//TWeakObjectPtr<class UTextBlock> ItemCount;

	//// 아이템 아이콘 이미지
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	//TWeakObjectPtr<class UImage> Icon;
};
