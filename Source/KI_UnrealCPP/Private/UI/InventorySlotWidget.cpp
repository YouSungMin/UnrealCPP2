// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/InventoryComponent.h"
#include "Framework/PickupFactorySubsystem.h"
#include "UI/InventoryDragDropOperation.h"
#include "UI/TemporarySlotWidget.h"
#include "UI/ItemDetailInfoWidget.h"
#include "Item/PickupItem.h"
#include "Blueprint/SlateBlueprintLibrary.h"

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
	// 드래그 앤 드롭이 실패로 끝났다.
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	UInventoryDragDropOperation* invenOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (invenOp && invenOp->ItemData.IsValid())
	{
		//UE_LOG(LogTemp, Log, 
		//	TEXT("DragCancelled : 바닥에다가 (%s)아이템을 버려야 한다."), 
		//	*(invenOp->ItemData->ItemName.ToString()));		

		APlayerController* playerController = GetOwningPlayer();
		if (playerController)
		{
			FVector2D absolutePosition = InDragDropEvent.GetScreenSpacePosition();	// 마우스의 스크린 좌표(절대좌표) 가져오기
			FVector2D pixelPosition;
			FVector2D viewportPosition;
			USlateBlueprintLibrary::AbsoluteToViewport(this, absolutePosition, pixelPosition, viewportPosition);
			//UE_LOG(LogTemp, Log, TEXT("Screen : %s"), *absolutePosition.ToString());
			//UE_LOG(LogTemp, Log, TEXT("Pixel : %s"), *pixelPosition.ToString());
			//UE_LOG(LogTemp, Log, TEXT("Viewport : %s"), *viewportPosition.ToString());

			FVector worldLocation;
			FVector worldDirection;
			if (playerController->DeprojectScreenPositionToWorld(
				pixelPosition.X,
				pixelPosition.Y,
				worldLocation, worldDirection))	// 스크린 좌표를 월드 좌표로 변환
			{
				FVector start = worldLocation;						// 시작점은 worldLocation(카메라 위치)
				FVector end = start + worldDirection * 10000.0f;	// 끝점은 시작점에서 worldDirection방향으로 100m 이동한 곳

				FVector spawnLocation;		// 아이템이 생성될 위치를 저장할 변수		
				UWorld* world = GetWorld();
				FHitResult hitResult;
				if (world->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility))	// LineTraceSingleByChannel
				{
					// LineTrace 성공										
					spawnLocation = hitResult.Location;
				}
				else
				{
					spawnLocation = end;
				}

				// 일정 거리 이상 떨어지지 않게 생성 위치 조절
				const float maxDistance = 500;
				FVector playerLocation = GetOwningPlayerPawn()->GetActorLocation();
				if (FVector::DistSquared2D(playerLocation, spawnLocation) > maxDistance * maxDistance)
				{
					FVector direction = (spawnLocation - playerLocation).GetUnsafeNormal2D();

					spawnLocation = playerLocation + direction * maxDistance;
					FVector DownStart = spawnLocation + FVector(0, 0, 10000.0f); // 100m 위에서
					FVector DownEnd = spawnLocation - FVector(0, 0, -10000.0f);  // 100m 아래까지
					FHitResult GroundHit;
					if (world->LineTraceSingleByChannel(GroundHit, DownStart, DownEnd, ECollisionChannel::ECC_Visibility))
					{
						spawnLocation = GroundHit.Location;
					}
				}

				// 실제 생성 시작
				UPickupFactorySubsystem* pickupFactory = world->GetSubsystem<UPickupFactorySubsystem>();
				for (int32 i = 0; i < invenOp->Count; i++)
				{
					FVector location = spawnLocation + FVector::UpVector * 100.0f;
					FVector2D randCircle = FMath::RandPointInCircle(30.0f);
					location.X += randCircle.X;
					location.Y += randCircle.Y;
					pickupFactory->SpawnPickup(invenOp->ItemData->ItemCode, location);
				}
			}
		}
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

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (SlotData && !SlotData->IsEmpty())
	{
		OnSlotEnter.Broadcast(Index);

	}
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (SlotData && !SlotData->IsEmpty())
	{
		OnSlotLeave.Broadcast();
	}
}
