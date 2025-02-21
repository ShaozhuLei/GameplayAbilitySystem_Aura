// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	//具体在蓝图中实现
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bBlockHit, bool bCriticalHit);
	
};
