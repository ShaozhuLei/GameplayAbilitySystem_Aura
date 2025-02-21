// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	/*想要火球在服务器中生成，而不是在客户端*/
	//检查当前是否是在服务器端
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	//当该Avatar Actor有关联ASC并且还实现了该接口则可以获取他们的Socket Location
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		
		//计算一个起始发射位置到目标位置的一个旋转
		FRotator Rotator = (ProjectileTargetLocation - SocketLocation).Rotation();

		//同时需要发射物飞行轨迹平行于地面
		//Rotator.Pitch = 0.f;

		/*发射物将飞行在空中， 碰到障碍物时触发效果
		 * 发射位置设计为法杖，而不是角色本身
		 * 为角色创建一个能够提供自己武器位置的接口
		 */
		FTransform  SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		//设置发射物旋转
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		/*赋予发射物一个GA用来造成伤害,要利用ASC来封装Spec*/
		//先通过静态方法获取ASC
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location= ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		
		//利用GE,等级,ContextHandle来封装GE_SpecHandle
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		
		for (auto& Pair: DamageTypes)
		{
			const float SclableDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, SclableDamage);
		}
		
		//通过该Projectile所携带的Handle应用到Target上;此时SpecHandle携带着<Tag, Damage> pair; DamageEffectSpecHandle是发射物基类的属性
		Projectile->DamageEffectSpecHandle = SpecHandle;
		
		Projectile->FinishSpawning(SpawnTransform);
		
	}
}
