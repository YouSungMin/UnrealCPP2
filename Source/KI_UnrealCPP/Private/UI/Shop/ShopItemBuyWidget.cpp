// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemBuyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Data/ItemDataAsset.h"
#include "Player/InventoryOwner.h"

void UShopItemBuyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemCount)
	{
		ItemCount->SetHintText(FText::AsNumber(MinimumItemCount));
		ItemCount->OnTextChanged.AddDynamic(this, &UShopItemBuyWidget::OnItemCountTextChange);	// 변경이 있을 때
		ItemCount->OnTextCommitted.AddDynamic(this, &UShopItemBuyWidget::OnItemCountTextCommited);	// 변경을 확정했을 때(엔터 입력 후 , 포커스를 잃은 후)

	}
}

void UShopItemBuyWidget::OnItemCountTextChange(const FText& Text)
{
	UE_LOG(LogTemp,Log,TEXT("Changed : %s"), *Text.ToString());

	FString number = Text.ToString();
	if (number.IsNumeric())
	{
		int32 count = FCString::Atoi(*number);
		ItemCount->SetText(FText::AsNumber(count));
	}

}

void UShopItemBuyWidget::OnItemCountTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	UE_LOG(LogTemp, Log, TEXT("Commited : %s"), *Text.ToString());

	FString number = Text.ToString();
	if (number.IsNumeric())
	{
		int32 count = FCString::Atoi(*number);
		ItemCount->SetText(FText::AsNumber(count));
	}
	else
	{
		ItemCount->SetText(FText::AsNumber(MinimumItemCount));
	}
}
