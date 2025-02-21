// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate=FRotator(0.f, 400.0f, 0.f);

	//当设置为 true 时，角色的移动将被限制在一个指定的平面内，超出平面范围的运动将被禁止。
	GetCharacterMovement()->bConstrainToPlane = true;
	
	//当设置为 true 时，在游戏开始或角色初始化时，会将角色的位置“捕捉”到约束平面上。
	//这对于确保角色的位置完全符合平面约束非常有用，避免初始位置偏离平面。
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilties();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init ability actor info for the client
	InitAbilityActorInfo();
	
}

int32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

//初始化AbilityActorInfo
void AAuraCharacter::InitAbilityActorInfo()
{
	//获取Player State
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	//检测 player_state
	check(AuraPlayerState);
	//通过player state，GetAbilitComponent来调用InitAbilityActorInfo()
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this); 

	/*调用AuraAbilitySystemComponent中的一个函数(GE 代理绑定函数), */
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	/*初始化Overlay UI. 为初始化过程提供 Controller, PlayerState, ASC, AttibuteSet */
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
	
}
