// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GoldPanelWidget.h"
#include "Components/TextBlock.h"

void UGoldPanelWidget::SetGold(int32 InGold)
{
	GoldText->SetText(FText::AsNumber(InGold));
}
