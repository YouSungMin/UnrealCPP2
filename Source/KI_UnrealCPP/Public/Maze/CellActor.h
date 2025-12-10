// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maze/CellData.h"
#include "CellActor.generated.h"

UCLASS()
class KI_UNREALCPP_API ACellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACellActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void InitalizeCell(FCellData* InCellData);

	inline float GetCellHalfSize() const { return CellHalfSize; }
protected:
	// 벽과 바닥용 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Cell")
	TObjectPtr<UStaticMeshComponent> BaseMesh = nullptr;

	// 4방향에 있는 문 메시의 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Cell")
	TArray<TObjectPtr<UStaticMeshComponent>> GateMeshArray;

	// 셀 한변의 길이의 절반
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Cell")
	float CellHalfSize = 1000.0f;

	//문 두께의 절반
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Cell")
	float CellHalfThickness = 50.0f;

private:
	EDirectionType Path = EDirectionType::None;
};
