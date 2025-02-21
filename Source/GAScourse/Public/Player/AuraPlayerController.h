// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputConfig.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class UInputComponent;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraAbilitySystemComponent;
class USplineComponent;
/**
 * 
 */
UCLASS()
class GASCOURSE_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	/*展示玩家对NPC造成的伤害数值,Client代表代表这个函数是即服务器端会调用该函数，并在客户端上执行
	 * Reliable保证可靠传输，即如果数据丢失，UE 会自动重试，确保最终能到达目标端。
	 * 适用于关键性 RPC，比如任务完成、存档数据更新等，保证数据不会丢失。
	 */
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockHit, bool bCriticalHit);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere, category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	//前向声明，如果文件中只需要声明指向 UInputAction 的指针或引用，而不需要访问其成员，则不需要包含完整的头文件。
	UPROPERTY(EditAnywhere, category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed(){bShiftKeyDown = true;};
	void ShiftReleased(){bShiftKeyDown = false;};
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	//角色亮光展示
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;

	//按键事件
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	//操作配置 数据资产类
	UPROPERTY(EditDefaultsOnly, category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	//PlayerController中定义一个ASC指针
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressedThreshold = 0.5f;
	bool bAutoRunning = false;
	//是否是在瞄准敌人
	bool bTargeting = false;
	//离目标还有多远
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	//是一个UI Component,其中携带者伤害数字的Widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

private:
	
	
};
