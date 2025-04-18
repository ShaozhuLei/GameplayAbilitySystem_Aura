// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Data/LevelUpInfo.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FonPlayerStateChangedDelegate, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StatValue*/, bool /*bLevelUp*/)

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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FonPlayerStateChangedDelegate OnXPChangedDelegate;
	FOnLevelChanged OnLevelChangedDelegate;
	FonPlayerStateChangedDelegate OnAttributePointsChangedDelegate;
	FonPlayerStateChangedDelegate OnSpellPointsChangedDelegate;

	//主要用于指示编译器将函数进行内联化，以提升性能。
	FORCEINLINE int32 GetPlayerLevel() const {return Level;};
	FORCEINLINE int32 GetPlayerXP() const {return XP;};
	FORCEINLINE int32 GetAttributePoints() const {return AttributePoints;};
	FORCEINLINE int32 GetSpellPoints() const {return SpellPoints;};

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InAttributePoints);
	void AddToSpellPoints(int32 InSpellPoints);

	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void SetAttributePoints(int32 InAttributePoints);
	void SetSpellPoints(int32 InSpellPoints);
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributesPoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributesPoints(int32 OldAttributesPoints);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	
};

