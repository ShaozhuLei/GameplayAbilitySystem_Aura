// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MyAttributeMenuWidgetController.h"

#include <utility>
#include <utility>

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Animation/AnimAttributes.h"
#include "Data/AttributeInfo.h"
#include "AuraGameplayTags.h"
void UMyAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (auto& Pair: AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		}
		);
	}
	
}

void UMyAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(AttributeTag);
	Info.AttribueValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

void UMyAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	check(AttributeInfo);
	for (auto& Pair: AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
}

