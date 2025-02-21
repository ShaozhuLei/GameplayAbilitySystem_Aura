// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//对widget广播一些“初始”属性值
	const UAuraAttributeSet* AuraAttributes = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AuraAttributes->GetHealth());
	
	OnMaxHealthChanged.Broadcast(AuraAttributes->GetMaxHealth());
	
	OnManaChanged.Broadcast(AuraAttributes->GetMana());
	
	OnMaxManaChanged.Broadcast(AuraAttributes->GetMaxMana());
	
	
	
}

// HUD类对此函数进行调用
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	//用于监听指定属性（Gameplay Attribute）的值变化。当属性值发生变化时，触发相关的回调函数
	//GetGameplayAttributeValueChangeDelegate(AuraAttributes->GetHealthAttribute()) 返回一个代理
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});

	//GetMaxHealth() = 0
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
  
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	/*代理变量EffectAssetTags可以绑定Lambda表达式对*/
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag: AssetTags)
			{
				//Tag 匹配机制：A.B 匹配 A为True; A 匹配 A.B为False(长的去匹配Tag的前缀)
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetTableDataRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetDelegate.Broadcast(*Row); 
				}
			}
		}

	);
}

