// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	
	//StartUpInputTag will be set for class default object for any given ability
	UPROPERTY(EditDefaultsOnly,Category="Input")
	FGameplayTag StartupInputTag;
	
};
