// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraHUD.generated.h"
class UMyAttributeMenuWidgetController;
class USpellMenuWidgetController;
/**
 * 
 */
UCLASS()
class GASCOURSE_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:

	//获取主UI 控制器, 参数为四项的结构体
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams WCParams);
	UMyAttributeMenuWidgetController* GetMyAttributeMenuWidgetController(const FWidgetControllerParams WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	
private:

	//AuraUserWidget 指针
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	//UserWidget基类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	/*SpellMenu WC*/
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
	/*SpellMenu WC*/
	
	/*OverlayWidgetController*/
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	/*End OverlayWidgetController*/
	
	//存放AttributeMenuWidgetController
	UPROPERTY()
	TObjectPtr<UMyAttributeMenuWidgetController> AttributeMenuWidgetController;

	//存放AttributeMenuWidgetController实体类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMyAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
