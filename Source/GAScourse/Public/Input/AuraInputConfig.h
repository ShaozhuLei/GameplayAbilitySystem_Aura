// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
	
};
/**
 * 
 */
UCLASS()
class GASCOURSE_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	//通过Tag来查找输入行为
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag InputTag, bool blogNotFound = false) const;
	
	//用来存储输出行为的结构体数组
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
