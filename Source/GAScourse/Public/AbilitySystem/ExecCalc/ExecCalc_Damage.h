// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:	
	UExecCalc_Damage();

	//什么类型的Execution将发生; FGameplayEffectCustomExecutionParameters是一个结构体
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
