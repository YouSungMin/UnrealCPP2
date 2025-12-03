// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemDetailInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemDetailInfoWidget::SetInfoIconImage(UTexture2D* InTexture)
{
	ItemIconImage->SetBrushFromTexture(InTexture);
}

void UItemDetailInfoWidget::SetDetailInfoText(FText InName, FText InPrice, FText InInfo)
{
	ItemNameText->SetText(InName);
	ItemPriceText->SetText(InPrice);
	ItemInfoText->SetText(InInfo);
}
