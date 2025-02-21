// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class GASCOURSE_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	//用于网络同步的一个函数，通常用于定义哪些变量需要在网络上进行同步以及如何进行同步。
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;};

	//主要用于指示编译器将函数进行内联化，以提升性能。
	FORCEINLINE int32 GetPlayerLevel() const {return Level;};
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
};

