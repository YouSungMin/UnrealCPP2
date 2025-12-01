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
	// 위젯이 보여줄 데이터 세팅
	void InitializeSlot(int32 InIndex, struct FInvenSlot* InSlotData);

	// 설정된 데이터를 기반으로 위젯에서 표시하는 내용을 갱신
	void RefreshSlot() const;

protected:
	void ClearSlotWidget() const;
protected:
	// 아이템 소지 갯수 텍스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UTextBlock> CountText;
	
	// 아이템 최대 갯수 텍스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UTextBlock> MaxCountText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UTextBlock> SeparateText;

	// 아이템 아이콘 이미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage;
private:
	FInvenSlot* SlotData = nullptr;

	int32 Index = -1;

};
