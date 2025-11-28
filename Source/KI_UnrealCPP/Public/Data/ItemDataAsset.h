// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/CommonEnums.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	EItemCode ItemCode = EItemCode::Figure;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	FText ItemName = FText::FromString(TEXT("아이템"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	FText ItemDescription = FText::FromString(TEXT("아이템설명"));;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData", meta = (ClampMin = "1"))
	int32 ItemPrice = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData", meta = (ClampMin = "1"))
	int32 ItemMaxStackCount = 1;

};
