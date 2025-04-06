// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;

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
	virtual int32 FindLevelForXP_Implementation(int32 InXP)const override;
	virtual int32 GetXP_Implementation()const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level)const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level)const override;
	virtual void AddToPlayerLevel_Implementation(const int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;
	
	/*End Player Interface*/

	/*CombatInterface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	/*End CombatInterface*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	virtual void InitAbilityActorInfo() override;

	//在特定坐标激活Niagara特效, 且特效在Client也可以看到
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};


