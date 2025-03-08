// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/*Player Interface*/
	virtual void AddToXP_Implementation(const int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 GetAttributePointsReward_Implementation() override;
	virtual int32 GetSpellPointsReward_Implementation() override;
	virtual void AddToPlayerLevel_Implementation(const int32 InPlayerLevel) override;
	virtual void AddToAttributePointPoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	
	/*End Player Interface*/

	/*CombatInterface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	/*End CombatInterface*/

private:
	virtual void InitAbilityActorInfo() override;
};
