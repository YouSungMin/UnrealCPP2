// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Usable/UsableItemDataAsset.h"
#include "UsableItemDataAsset_Pouch.generated.h"

/**
 * 
 */
UCLASS()
class KI_UNREALCPP_API UUsableItemDataAsset_Pouch : public UUsableItemDataAsset
{
	GENERATED_BODY()
public:
	virtual void UseItem_Implementation(AActor* InTarget) override;
};
