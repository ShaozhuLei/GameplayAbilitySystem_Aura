// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"


const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag InputTag, bool blogNotFound) const
{
	for (const FAuraInputAction& Action: AbilityInputActions)
	{
		if (Action.InputTag == InputTag && Action.InputAction)
		{
			return Action.InputAction;
		}
	}
	if (blogNotFound) UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	return nullptr;
}
