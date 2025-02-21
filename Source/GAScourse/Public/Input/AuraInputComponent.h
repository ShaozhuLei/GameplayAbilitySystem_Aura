// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	//从AuraPlayerController中对此函数进行调用
	template<class UserClass, typename PressedFunType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFunType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
	{
		check(InputConfig);

		//遍历技能数组 其中Action是一个Struct
		for (const FAuraInputAction& Action: InputConfig->AbilityInputActions)
		{
			//Action.InputAction = UInputAction; Action.InputTag = FGameplayTag
			if (Action.InputAction && Action.InputTag.IsValid())
			{
				/*Action.InputTag这个参数会被塞到PressedFunc中进行传递(PressedFunc(Action.InputTag))
				 *将所有的可触发按键都具有Started, Completed 和 Triggerd三个阶段
				*/
				if (PressedFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
				}
				if (ReleasedFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
				}
				if (HeldFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
				}
			}
		}
	}
};
