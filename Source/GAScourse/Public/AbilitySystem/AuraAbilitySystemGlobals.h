// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	//重写该函数，原函数是返回默认FGameeplayEffectContext类，现在返回自定义的
	GENERATED_BODY()
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
