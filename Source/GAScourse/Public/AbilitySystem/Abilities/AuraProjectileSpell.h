// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class GASCOURSE_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	//重写"能力激活函数"
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//参数是目标位置
	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	//需要一个能够生成发射物的Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
  