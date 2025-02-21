// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxHealth();

	//要返回计算后的最终结果
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	//捕获哪些属性
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
