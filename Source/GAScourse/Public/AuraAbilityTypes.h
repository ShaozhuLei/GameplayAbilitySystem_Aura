#pragma once

#include "GameplayEffectTypes.h"
#include <stdbool.h>
#include "AuraAbilityTypes.generated.h"
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObjectPtr = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
 
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;
 
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;
 
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;
 
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();
 
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;
 
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;
 
	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;
 
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockBackMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
};
/*自定义FGamgeplayEffectContext类*/
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext: public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockHit()const{return bIsBlockHit;}
	bool IsCriticalHit()const{return bIsCriticalHit;}
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	
	void SetIsBlockHit(bool isBlockHit){bIsBlockHit = isBlockHit;}
	void SetIsCriticalHit(bool isCriticalHit){bIsCriticalHit = isCriticalHit;}
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType){DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	void SetKnockbackForce(const FVector& InForce){KnockbackForce = InForce;}
	void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
	
	/*类似于 GetClass() 但适用于 USTRUCT。用于序列化（Serialization）、网络复制（Replication）等场景*/
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		//FAuraGameplayEffectContext是我们在上面生命的结构体
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/*用于自定义数据的序列化（Serialization）和反序列化（Deserialization）以便在 服务器和客户端之间高效传输数据。*/
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsBlockHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
 
	UPROPERTY()
	float DebuffDamage = 0.f;
 
	UPROPERTY()
	float DebuffDuration = 0.f;
 
	UPROPERTY()
	float DebuffFrequency = 0.f;
 
	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;
 
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;
 
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;
 
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

/*
 *自定义 USTRUCT 的特性（Traits）的模板结构体。
 *它的作用是为 USTRUCT 类型启用特定功能，如网络序列化（WithNetSerializer）、复制（WithCopy）等。
 */
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		/*让 FAuraGameplayEffectContext 结构体支持网络序列化（NetSerialize）。必须实现 NetSerialize() 函数，否则会编译失败！*/
		WithNetSerializer = true,
		/*允许 FAuraGameplayEffectContext 进行 二进制复制（memcpy）。适用于 没有指针（UObject）或复杂动态数据（TArray、FString）* 的 USTRUCT。*/
		WithCopy = true
	};
};
