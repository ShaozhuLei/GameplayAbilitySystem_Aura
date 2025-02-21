// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//C++实现的将自己的 InWidgetController 赋给 成员变量WidgetController 的函数
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:

	//为蓝图设计的设置WidgetController的函数(具体在蓝图中实现)
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
