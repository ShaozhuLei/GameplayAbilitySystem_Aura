// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "UI/WidgetController/MyAttributeMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UMyAttributeMenuWidgetController* AAuraHUD::GetMyAttributeMenuWidgetController(const FWidgetControllerParams WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UMyAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	//以下两个变量均来自BP_AuraHUD中的添加
	checkf(OverlayWidgetClass, TEXT("Overlay Widget class uninitialized, fill BP_AuraHud"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, Plz fill out BP_AuraHUD"))

	//获取OverlayWidget(主UI)
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	

	//获取WidgetController
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(WidgetController);

	//这里开始调用controller的广播方法，因为此时OverlayWidget以及确定自己的controller是谁了
	WidgetController->BroadcastInitialValues();
	
	OverlayWidget->AddToViewport();
}

