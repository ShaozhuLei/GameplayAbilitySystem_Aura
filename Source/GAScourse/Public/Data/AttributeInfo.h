// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttribueValue = 0.f;
};

/**
 * 
 */
UCLASS()
class GASCOURSE_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	//接受Tag， 返回一个对应的AttributeInformation的结构体
	FAuraAttributeInfo FindAttributeInfoByTag(const FGameplayTag& Tag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformations;
};
