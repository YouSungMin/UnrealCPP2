// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "InputMappingContext.h"
#include "Player/InventoryComponent.h"
#include "Player/ActionCharacter.h"
#include "NPC/Merchant.h"

void AActionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());	// Subsystem 가져오기 시도
	if (Subsystem)	// Subsystem이 null이 아니면
	{
		Subsystem->AddMappingContext(DefaultMappingContext, GameInputPriority);
	}

	PlayerCameraManager->ViewPitchMax = VewPitchMax;
	PlayerCameraManager->ViewPitchMin = VewPitchMin;
}

void AActionPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AActionCharacter* player = Cast<AActionCharacter>(aPawn);
	if (player)
	{
		InventoryComponent = player->GetInventoryComponent();
		if (InventoryWidget.IsValid())
		{
			InventoryWidget->InitializeInventoryWidget(InventoryComponent.Get());
		}
	}
}

void AActionPlayerController::OnUnPossess()
{
	if (InventoryWidget.IsValid())
	{
		InventoryWidget->ClearInventoryWidget();
	}
	InventoryComponent = nullptr;

	Super::OnUnPossess();
}

void AActionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(InputComponent);
	if (enhanced)	// 입력 컴포넌트가 향상된 입력 컴포넌트일 때
	{
		//UE_LOG(LogTemp, Log, TEXT("바인드 성공"));
		enhanced->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AActionPlayerController::OnLookInput);
		enhanced->BindAction(IA_InventoryOnOff, ETriggerEvent::Started, this, &AActionPlayerController::OnInventoryOnOff);
	}
}

void AActionPlayerController::OpenShopWidget(AMerchant* TargetMerchant)
{
	if (MainHudWidget.IsValid() && !MainHudWidget->IsShopOpen())
	{
		UE_LOG(LogTemp, Log, TEXT("OpenShopWidget"));
		MainHudWidget->OpenInventory();
		MainHudWidget->OpenShop(TargetMerchant->GetItemList());
		FreezePlayer();

		SetViewTargetWithBlend(TargetMerchant, 1, EViewTargetBlendFunction::VTBlend_Cubic);
	}
}

void AActionPlayerController::CloseShopWidget()
{
	if (MainHudWidget.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("CloseShopWidget"));
		UnFreezePlayer();
		MainHudWidget->CloseInventory();
		MainHudWidget->CloseShop();

		SetViewTargetWithBlend(this->GetPawn(), 1, EViewTargetBlendFunction::VTBlend_Cubic);
	}
}

void AActionPlayerController::OnLookInput(const FInputActionValue& InValue)
{
	FVector2D lookAxis = InValue.Get<FVector2D>();
	//UE_LOG(LogTemp, Log, TEXT("OnLookInput : %s"), *lookAxis.ToString());
	AddYawInput(lookAxis.X);
	AddPitchInput(lookAxis.Y);
}

void AActionPlayerController::OnInventoryOnOff()
{
	if (MainHudWidget.IsValid())
	{
		if (MainHudWidget->GetInventoryState() == EOpenState::Open)
		{
			CloseInventoryWidget();
		}
		else
		{
			OpenInventoryWidget();
		}
	}
}

void AActionPlayerController::FreezePlayer()
{

	FInputModeGameAndUI inputMode;
	inputMode.SetWidgetToFocus(MainHudWidget->TakeWidget());		// 위젯에 포커스 주기
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	// 마우스 커서가 뷰포트를 벗어날 수 있게 설정
	inputMode.SetHideCursorDuringCapture(false);	// 마우스가 눌러졌을 때도 커서가 보이도록 설정
	SetInputMode(inputMode);	// InputMode를 플레이어 컨트롤러에 적용

	bShowMouseCursor = true;

	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);

	//SetPause(true); 
}

void AActionPlayerController::UnFreezePlayer()
{
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	//SetPause(false);

	FInputModeGameOnly inputMode;
	SetInputMode(inputMode);	// InputMode를 플레이어 컨트롤러에 적용

	bShowMouseCursor = false;
}

void AActionPlayerController::OpenInventoryWidget()
{
	if (MainHudWidget.IsValid() && !MainHudWidget->IsInventoryOpen())
	{
		MainHudWidget->OpenInventory();
		FreezePlayer();
	}
}

void AActionPlayerController::CloseInventoryWidget()
{
	if (MainHudWidget.IsValid())
	{
		UnFreezePlayer();
		MainHudWidget->CloseInventory();
	}
}

inline void AActionPlayerController::InitializeMainHudWidget(UMainHudWidget* InWidget)
{
	if (InWidget)
	{
		MainHudWidget = InWidget;

		// MainHudwidget의 Inventory의 닫힘 델리게이트에 함수 연결
		FScriptDelegate delegate;
		delegate.BindUFunction(this, "CloseInventoryWidget");
		MainHudWidget->AddToInventoryCloseDelegate(delegate);

		FScriptDelegate delegateshop;
		delegateshop.BindUFunction(this, "CloseShopWidget");
		MainHudWidget->AddToShopCloseDelegate(delegateshop);

		InventoryWidget = MainHudWidget->GetInventoryWidget();

		if (InventoryWidget.IsValid())
		{
			InventoryWidget->InitializeInventoryWidget(InventoryComponent.Get());
		}
	}
}

void AActionPlayerController::TestChangeInventoryTarget(UInventoryComponent* NewTarget)
{
	if (NewTarget)
	{
		InventoryWidget->InitializeInventoryWidget(NewTarget);
	}
}
