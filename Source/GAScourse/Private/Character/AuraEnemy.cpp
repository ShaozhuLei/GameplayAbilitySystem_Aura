// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAScourse/GAScourse.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}


void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	/*
	 * 第162节中提到多人模式崩溃问题，原因是只能在服务器上调用GameMode，若在Client上调用GameMode则返回nullptr
	 * 于是在此要对是否是在服务器端进行检查
	 */
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}
	
	
	//为了设置能够在BP_Widget中获取敌人的实例，需要设置widgetcontroller
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	//获取AS
	if (UAuraAttributeSet* AuraASC = Cast<UAuraAttributeSet>(AttributeSet))
	{
		//对HP改动代理进行绑定
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraASC->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraASC->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		//存在ASC的环境下对到来的Tag进行监听;RegisterGameplayTagEvent返回一个绑定回调函数的Delegate
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);

		//给敌人单位赋予初始值
		OnHealthChanged.Broadcast(AuraASC->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraASC->GetMaxHealth());
	}
	//利用Lambda表达式进行代理绑定（HP，MaxHP都需要进行绑定）
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallBackTag, const int32 NewCounts)
{
	bHitReacting = NewCounts > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting? 0.f : BaseWalkSpeed;
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	
	
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttribute(this, CharacterClass, Level, AbilitySystemComponent);
}
