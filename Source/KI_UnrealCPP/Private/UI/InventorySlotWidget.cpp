// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/InventoryComponent.h"
#include "Framework/PickupFactorySubsystem.h"
#include "UI/InventoryDragDropOperation.h"
#include "UI/TemporarySlotWidget.h"
#include "Item/Pickup.h"

void UInventorySlotWidget::InitializeSlot(UInventoryComponent* InInventoryComponent, int32 InIndex)
{
	if (InInventoryComponent)
	{
		TargetInventory = InInventoryComponent;
		Index = InIndex;
		SlotData = InInventoryComponent->GetSlotData(InIndex);
		OnSlotRightClick.BindUFunction(TargetInventory.Get(), "UseItem");	// 인벤토리 컴포넌트에 있는 UseItem과 바인딩

		RefreshSlot();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InInventoryComponent가 nullptr 입니다."));
	}
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

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	//UE_LOG(LogTemp,Log,TEXT("DragDetected : %d Slot"), this->Index);

	
	// 기본 데이터 세팅
	UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>();
	DragOp->StartIndex = this->Index;
	DragOp->ItemData = SlotData->ItemData;
	DragOp->Count = SlotData->GetCount();

	// 비주얼 위젯 만들기
	UTemporarySlotWidget* DragTemporaryWidget = CreateWidget<UTemporarySlotWidget>(this, TargetInventory->GetTemporarySlotWidgetClass());
	DragTemporaryWidget->SetItemIconImage(SlotData->ItemData->ItemIcon);
	DragTemporaryWidget->SetCountText(SlotData->GetCount());

	DragOp->DefaultDragVisual = DragTemporaryWidget;

	OutOperation = DragOp;

	TargetInventory->ClearSlotAtIndex(Index);
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventoryDragDropOperation* invenOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (invenOp && invenOp->ItemData.IsValid())
	{
		//UE_LOG(LogTemp, Log, TEXT("Drop : %d Slot에 옮기기 %s"), Index,*(invenOp->ItemData->ItemName.ToString()));
		
		if (SlotData->IsEmpty())
		{
			// 빈슬롯
			TargetInventory->SetItemAtIndex(Index, invenOp->ItemData.Get(), invenOp->Count);
		}
		else
		{
			if (SlotData->ItemData == invenOp->ItemData)
			{
				int32 count = FMath::Min(SlotData->GetRemainingCount(), invenOp->Count);
				TargetInventory->UpdateSlotCount(Index, count);

				int32 returnCount = invenOp->Count - count;
				if (returnCount > 0)
				{
					TargetInventory->SetItemAtIndex(invenOp->StartIndex,invenOp->ItemData.Get(),returnCount);
				}
			}
			else
			{
				TargetInventory->SetItemAtIndex(invenOp->StartIndex, SlotData->ItemData.Get(), SlotData->GetCount());
				TargetInventory->SetItemAtIndex(Index, invenOp->ItemData.Get(), invenOp->Count);
			}
		}

		return true;
	}
	return false;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	UInventoryDragDropOperation* invenOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (invenOp)
	{
		//UE_LOG(LogTemp, Log, TEXT("DragCancelled: 바닥에다가 (%s) 아이템을 버려야 한다"),*(invenOp->ItemData->ItemName.ToString()));
		APickup* pickup = GetWorld()->GetSubsystem<UPickupFactorySubsystem>()->SpawnPickup(invenOp->ItemData->ItemCode,FVector(0,0,0));

		RefreshSlot();
	}
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))	//마우스 오른쪽 버튼이 눌렸는지 확인
	{
		if (SlotData->ItemData)
		{
			//UE_LOG(LogTemp,Log,TEXT("Widget %d Slot : Right click(%s)"),Index,*SlotData->ItemData->ItemName.ToString());
			OnSlotRightClick.ExecuteIfBound(Index);
		}
		else
		{
			//UE_LOG(LogTemp,Log,TEXT("Widget %d Slot : Right click(empty)"),Index);
		}
		return FReply::Handled();	// 이 마우스 클릭은 완료되었다고 전달.
	}
	else if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))	// 마우스 왼쪽 버튼 눌렸는지 확인
	{
		if (SlotData->ItemData)
		{
			return FReply::Handled().DetectDrag(TakeWidget(),EKeys::LeftMouseButton);
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);	// 나는 처리안했다, 부모 or 다른 위젯이 처리
}
