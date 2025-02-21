// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	//返回一个
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	//检查是否Locally Controlled, 如果当前在客户端则返回true
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		/*此时是在服务器端，需要接受Target Data*/
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		//Returns TargetDataSet delegate for a given Ability/PredictionKey pair. 此时要通知服务器要等待Target Data过来
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		//如果CallReplicatedTargetDataDelegatesIfSet返回的是false则说明数据还未到达服务器
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		//指示服务器等待玩家数据
		if (!bCalledDelegate)
		{
			//等待数据
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	//建立一个Scoped Creation Window
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	//创建Target Data. 当是LocallyControlled的话 创建FGameplayAbilityTargetData_SingleTargetHit对象
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);
	
	/*向服务器发送Target Data.*/
	//GetActivationPredictionKey() 是Original Prediction Key; AbilitySystemComponent->ScopedPredictionKey是当前的Prediction Key
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	//在此环节需要检查是否满足广播的条件,GA是否还在运行当中
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//这里不再广播点击位置的Vector，而是广播打包好的Target Data
		ValidData.Broadcast(DataHandle);
	}
}

//接受TargetData时，进行调用
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	//该行代码为通知ASC已接受到Target Data，不需要再对Target Data进行存储了从而直接进行广播
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	//同样，在广播之前进行检查
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//这里不再广播点击位置的Vector，而是广播打包好的Target Data
		ValidData.Broadcast(DataHandle);
	}
}

