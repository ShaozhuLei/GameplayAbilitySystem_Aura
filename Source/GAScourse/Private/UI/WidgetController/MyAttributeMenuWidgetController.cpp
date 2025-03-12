// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MyAttributeMenuWidgetController.h"

#include <utility>
#include <utility>

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Animation/AnimAttributes.h"
#include "Data/AttributeInfo.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UMyAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	for (auto& Pair: GetAuraAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		}
		);
	}
	
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](const int32 AttributePoints){
			AttributePointChangedDelegates.Broadcast(AttributePoints);
		}
	);
	
}

void UMyAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* ASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	ASC->UpgradeAttribute(AttributeTag);
}

void UMyAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                              const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(AttributeTag);
	Info.AttribueValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);

	AttributePointChangedDelegates.Broadcast(CastChecked<AAuraPlayerState>(PlayerState)->GetAttributePoints());
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

