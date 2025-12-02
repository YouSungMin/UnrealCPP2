// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/InventoryComponent.h"

void UInventorySlotWidget::InitializeSlot(int32 InIndex, FInvenSlot* InSlotData)
{
	Index = InIndex;
	SlotData = InSlotData;

	RefreshSlot();
}

void UInventorySlotWidget::RefreshSlot() const
{
	if (SlotData && !SlotData->IsEmpty())
	{
		ItemIconImage->SetBrushFromTexture(SlotData->ItemData->ItemIcon);
		ItemIconImage->SetBrushTintColor(FLinearColor(1,1,1,1));

		CountText->SetText(FText::AsNumber(SlotData->GetCount()));
		MaxCountText->SetText(FText::AsNumber(SlotData->ItemData->ItemMaxStackCount));

		SeparateText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MaxCountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		ClearSlotWidget();
	}
}

void UInventorySlotWidget::ClearSlotWidget() const
{
	ItemIconImage->SetBrushFromTexture(nullptr);
	ItemIconImage->SetBrushTintColor(FLinearColor::Transparent);
	SeparateText->SetVisibility(ESlateVisibility::Hidden);
	CountText->SetVisibility(ESlateVisibility::Hidden);
	MaxCountText->SetVisibility(ESlateVisibility::Hidden);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))	//마우스 우클릭 확인
	{
		if (SlotData->ItemData)
		{
			UE_LOG(LogTemp,Log,TEXT("Widget %d Slot : Right click(%s)"),Index,*SlotData->ItemData->ItemName.ToString());
			OnSlotRightClick.ExecuteIfBound(Index);
		}
		else
		{
			UE_LOG(LogTemp,Log,TEXT("Widget %d Slot : Right click(empty)"),Index);
		}
		return FReply::Handled();	// 이 마우스 클릭은 완료되었다고 전달.
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);	// 나는 처리안했다, 부모 or 다른 위젯이 처리
}
