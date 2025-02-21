// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	//遍历角色的技能列表
	for (const TSubclassOf<UGameplayAbility> AbilityClass: StartupAbilities)
	{
		/**
		 * FGameplayAbilitySpec 可激活的AbilitySpec，
		 * 托管在能力系统组件上。它既定义了能力的内容（什么类别、什么级别、输入绑定等）
		 * 也保存了必须在被实例化/激活的能力之外保留的运行时状态。
		 */
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			//将AuraGameplayAbility中的StartupInputTag添加到GameplayAbilitySpec中的DynamicAbilityTags当中
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
		//GiveAbilityAndActivateOnce(AbilitySpec); //AbilitySpec则不能是Const
		
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	//获取一切可以触发的技能，返回一个TArray
	for (FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
	{
		//触发与InputTag一致的技能
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//标记该能力的输入状态为“已按下”。
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	//获取一切可以触发的技能，返回一个TArray
	for (FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
	{
		//触发与InputTag一致的技能
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//标记该能力的输入状态为“已释放”。
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}

