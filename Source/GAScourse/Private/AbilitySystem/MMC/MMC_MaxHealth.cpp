// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//构造函数中对属性进行获取
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	//区分属性是从source还是从target中来
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	//是否技能效果GE生成瞬间对属性进行捕获
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
	
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather Tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//将Tag进行集合
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	// 80 + 2.50 * 10 + 10 * 1 = 115
	return 85.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
