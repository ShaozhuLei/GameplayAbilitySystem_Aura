// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

//WidgetController 结构体包含playerController PlayerState ASC AttributeSet
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet;

	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		:PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS){}
};
/**
 * 
 */
UCLASS()
class GASCOURSE_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:

	//设置WidgetController的函数，向函数传输FWidgetControllerParams结构体，给成员变量进行赋值
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	//用来广播唯初始值
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	//将回调函数与依赖进行绑定
	virtual void BindCallbacksToDependencies();
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
