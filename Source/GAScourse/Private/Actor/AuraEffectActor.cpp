// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC == nullptr) return;
	check(GameplayEffectClass);
	/**
	*	EffectContextHandle 用来存储关于这个 Gameplay Effect（GE）应用过程中的上下文信息。
	*	EffectContextHandle 包含了：SourceObject：触发 Gameplay Effect 的来源。
	*	Instigator：施加效果的控制器或角色。
	*	TargetData：影响的目标列表。其他信息（如命中位置）。
	*/
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();

	//这个操作告诉 EffectContextHandle，当前对象（如技能或角色）是该效果的来源。
	EffectContextHandle.AddSourceObject(this);

	//GameplayEffectSpec 是 Gameplay Effect 的具体实例，包含了效果的定义、上下文和相关参数。
	//返回一个 FGameplayEffectSpecHandle，它是对 GameplayEffectSpec 的包装，用于描述即将应用的效果。
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);

	//EffectSpecHandle.Data.Get() 返回一个指向 FGameplayEffectSpec 的指针。
	//使用 * 解引用后，获得实际的 GameplayEffectSpec。
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	//判断技能持续时间类型是否是Infinite
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}
	

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (TargetASC == nullptr) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair: ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle: HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}


