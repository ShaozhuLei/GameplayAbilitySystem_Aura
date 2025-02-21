// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/AttributeInfo.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "MyAttributeMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo, Info);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GASCOURSE_API UMyAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	//暴露给蓝图，蓝图中为此变量赋予Data Asset
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
