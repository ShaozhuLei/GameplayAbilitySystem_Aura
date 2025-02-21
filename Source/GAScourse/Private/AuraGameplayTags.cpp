// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

//定义静态成员变量, 第一个 FAuraGameplayTags 是 类型声明，用于指定 GameplayTags 的数据类型。
//第二个 FAuraGameplayTags 是 作用域限定符，用于表明 GameplayTags 属于 FAuraGameplayTags 类。
//GameplayTags是一个静态成员变量
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	//对象.属性 = ....
	//然后向本地添加Tag
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Strength"), FString("Increase physical damage"));
	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Intelligence"), FString("Increase magic damage"));
	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Resilience"), FString("Increase Armor and Penetration"));
	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Vigor"), FString("Increase Health"));
	

	//Secondary Attribute
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"), FString("Reduce damage taken, improves Block Chance"));
	GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ArmorPenetration"), FString("Ignore percentage of Armor"));
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.BlockChance"), FString("Chance to cut incoming damage in half"));
	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChance"), FString("Chance to double the damage"));
	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitDamage"), FString("Bonus damage added when a critical hit is scored"));
	GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitResistance"), FString("Reduce critical damage taken"));
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.HealthRegeneration"), FString("Amount of health regeneration"));
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ManaRegeneration"), FString("Amount of mana regeneration"));
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"), FString("Max amount of health obtained"));
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxMana"), FString("Max amount of mana obtained"));

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Mouse left button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Mouse right button"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("Key 1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("Key 2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("Key 3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("Key 4"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Fire Damage"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), FString("Fire Damage"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Fire Damage"));

	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Fire"), FString("Fire Damage"));
	GameplayTags.Attribute_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Lightning"), FString("Fire Damage"));
	GameplayTags.Attribute_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Arcane"), FString("Fire Damage"));
	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Physical"), FString("Fire Damage"));

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attribute_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attribute_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attribute_Resistance_Physical);
	
	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Tag granted when hit Reacting"));
	
}
