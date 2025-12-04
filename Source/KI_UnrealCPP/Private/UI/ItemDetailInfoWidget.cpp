// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemDetailInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/ItemDataAsset.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UItemDetailInfoWidget::Open(UItemDataAsset* InItemData)
{
	SetInfo(InItemData);
	UpdateLocation();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UItemDetailInfoWidget::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}


void UItemDetailInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateLocation();
}

void UItemDetailInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
}

void UItemDetailInfoWidget::SetInfo(UItemDataAsset* InItemData)
{
	if (InItemData)
	{
		ItemNameText->SetText(InItemData->ItemName);
		ItemPriceText->SetText(FText::AsNumber(InItemData->ItemPrice));
		ItemDescription->SetText(InItemData->ItemDescription);
		ItemIconImage->SetBrushFromTexture(InItemData->ItemIcon);
	}
}

void UItemDetailInfoWidget::UpdateLocation()
{
	FVector2D mousePosition = UWidgetLayoutLibrary:: GetMousePositionOnViewport(GetWorld());

	CanvasSlot->SetPosition(mousePosition - ParentPosition);
}
