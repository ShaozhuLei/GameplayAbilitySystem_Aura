// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;


UCLASS()
class GASCOURSE_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;};

	//只在服务器上调用该函数
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;

	//控制所有客户端中角色的死亡
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;

	virtual FVector GetCombatSocketLocation() override;

	//定义Ability system component指针
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	//定义Attribute set的指针
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UFUNCTION()
	virtual void InitAbilityActorInfo();

	//基础属性GE
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttribute;

	UPROPERTY(Blueprintable, EditAnywhere, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttribute;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultVitalAttribute;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectclass, float Level) const;

	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilties();

	/*逐渐溶解效果*/
	void Dissolve();

	/*需蓝图实现的函数*/
	//角色本体消融
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	//角色武器消融
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

private:
	
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
