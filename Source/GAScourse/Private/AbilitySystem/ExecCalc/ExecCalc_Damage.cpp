// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	/*
	 *Armor是用于捕获这些属性的变量，但尚未定义其具体内容。
	 *
	*/
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	//<Tag, AttributeDef>
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Physical, PhysicalResistanceDef);
	}
};

//无论多少次调用该函数，DStatics都是固定的，因为是由static所修饰的
static const AuraDamageStatics& DamageStatics()
{
	//通过调用此函数来调用AuraDamageStatics结构体
	static AuraDamageStatics DStatics;
	return DStatics;
}


//构造函数
UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);

	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

/*FGameplayEffectCustomExecutionParameters提供所有执行所需的上下文。*/
void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/*通过设置属性，看是如何影响其他属性的
	 * 首先展示一些可用，可操作的属性
	 */
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC? SourceASC->GetAvatarActor(): nullptr;
	AActor* TargetAvatar = TargetASC? TargetASC->GetAvatarActor(): nullptr;
	//将通过接口获取实现了接口角色的等级
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	/*获取 FAggregatorEvaluateParameters&, Gather Tags from source and target
	* -CapturedSourceTags 是一个 FGameplayEffectAttributeCaptureSpecContainer
	* 它存储了 从 Source（来源，比如施法者、攻击者）捕获的 Tags。
	* -GetAggregatedTags():从所有捕获的 Attribute Capture Spec 中，获取并合并它们的 Gameplay Tags。
	*/
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	//将Tag进行集合，FAggregatorEvaluateParameters是一个结构体，可存储SourceTags，TargetTags。
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	//开始捕捉一些属性
	float Damage = 0;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair: FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("Tags [%s] not exist"), *ResistanceTag.ToString());
		FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;
		
		Damage += DamageTypeValue;
	}
	

	/*如果拥有BlockChance，获取BlockChance的属性值
	 * BlockChance：格挡成功减伤一半
	 */
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max(TargetBlockChance, 0.f);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	
	/*开始使用我们自定义的FGameplayEffectContext(已在函数库中封装好)*/
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	//格挡成功 伤害减半
	Damage = bBlocked? Damage * 0.5f: Damage;

	/*计算护甲*/
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max(TargetArmor, 0.f);

	/*护甲穿透, 破甲系数需参考UE的配表数据*/
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.f);

	//通过Aura函数库获取CharacterClassInfo
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	//从CT表中找对应的曲线
	const FRealCurve* ArmorPenetration = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("ArmorPenetration"), FString());
	//通过等级来查询对性的系数值，ArmorPenetrationCoefficient就是某一等级下对应的系数值
	const float ArmorPenetrationCoefficient = ArmorPenetration->Eval(SourceCombatInterface->GetPlayerLevel());
	

	//相同的方式获取有效护甲值
	const float EffectiveArmor = TargetArmor * (1 - (SourceArmorPenetration * ArmorPenetrationCoefficient)/100.f);
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient)/ 100.f;

	/*暴击相关数值获取*/
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);
	/*完成获取暴击相关数据获取*/

	/*暴击计算*/
	//读表
	const FRealCurve* CriticalHitResistance = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistance->Eval(SourceCombatInterface->GetPlayerLevel());
	
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	/*开始使用我们自定义的FGameplayEffectContext(已在函数库中封装好)*/
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	Damage = bCriticalHit? 2.f * Damage : Damage;
	
	// 设置最终计算结果
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
