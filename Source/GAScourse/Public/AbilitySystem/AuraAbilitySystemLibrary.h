// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UMyAttributeMenuWidgetController;
/**
 * 
 */
UCLASS()
class GASCOURSE_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySustemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidget(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static UMyAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	//初始化角色属性值的函数
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySustemLibrary|CharaterClassDefault")
	static void InitializeDefaultAttribute(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	//初始化角色技能
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySustemLibrary|CharaterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySustemLibrary|CharaterClassDefault")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySustemLibrary|GameplayEffect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySustemLibrary|GameplayEffect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	/*UPARAM(ref)强制将EffectContextHandle作为蓝图中的输入Pin,而不是输出Pin*/
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySustemLibrary|GameplayEffect")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool IsBlocked);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySustemLibrary|GameplayEffect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool IsCriticalHit);
};




