// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//这是一个Data Asset
	UPROPERTY(EditDefaultsOnly, Category="Character Class Default")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	
};
