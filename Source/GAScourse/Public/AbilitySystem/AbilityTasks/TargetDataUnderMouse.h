// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&,  DataHandle);
/**
 * 
 */
UCLASS()
class GASCOURSE_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:

	//获取鼠标点击区域的Target Data
	UFUNCTION(BlueprintCallable, Category="Ability|Task", meta = (DisplayName = "TagetDataUnderMouse" ,HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	//该模块的所有输出引脚都是Delegate形式的
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	
	virtual void Activate() override;
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
