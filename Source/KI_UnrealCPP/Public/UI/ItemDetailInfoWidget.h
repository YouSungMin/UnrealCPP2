// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "ItemDetailInfoWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UItemDetailInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Open(class UItemDataAsset* InItemData);

	UFUNCTION()
	void Close();
	inline void SetParentPosition(const FVector2D& InPosition){ ParentPosition  = InPosition;}
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
	virtual void NativeConstruct()override;
private:
	void SetInfo(class UItemDataAsset* InItemData);
	void UpdateLocation();
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemPriceText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;

private:
	APlayerController* playerController = nullptr;

	UCanvasPanelSlot* CanvasSlot = nullptr;

	FVector2D ParentPosition;
};
