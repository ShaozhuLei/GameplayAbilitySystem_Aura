// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/AbilityInfo.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfo(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info: AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if (bLogNotFound) UE_LOG(LogTemp, Error, TEXT("Ability info not found"));
	return FAuraAbilityInfo();
}
