#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"
/*自定义FGamgeplayEffectContext类*/
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext: public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockHit()const{return bIsBlockHit;}
	bool IsCriticalHit()const{return bIsCriticalHit;}
	
	void SetIsBlockHit(bool isBlockHit){bIsBlockHit = isBlockHit;}
	void SetIsCriticalHit(bool isCriticalHit){bIsCriticalHit = isCriticalHit;}
	
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
