// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Data/AttributeInfo.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
	//初始化Sbline,("Spline")是在蓝图中的名称
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//增强输入组件
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	//绑定移动行为映射
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	//绑定按键映射
	AuraInputComponent->BindAbilityActions(InputConfig,
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
}

//在 的网络 RPC（远程过程调用） 机制中，声明为 UFUNCTION(Client, Reliable) 的 客户端 RPC 函数
void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockHit, bool bCriticalHit)
{
	//决定展示那个widget
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		//outer是当前对象(UDamageTextComponent)的上级，也就是角色
		UDamageTextComponent* DamageText =  NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		//手动注册模块，CreateDefaultObject函数中已将该函数封装，但此处并非在BeginPlay中所以需要手动
		DamageText->RegisterComponent();
		//UI组件挂载到角色的跟组件上，挂载方式是 KeepRelativeTransform
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		//弹出来的伤害数值不需要跟随目标移动，而是停留原地
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//SetDamageText()是DamageTextComponent雷少铸自己写的函数，设置数值字体用的
		DamageText->SetDamageText(DamageAmount, bBlockHit, bCriticalHit);
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		//需要角色在Spline上，而不是从犯Spline上有偏移
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		//拿到方向后，决定往哪个方向开始移动
		ControlledPawn->AddMovementInput(Direction);

		//检查到目的地的距离，是否在可接受的半径内
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			//到达目标点附近时
			bAutoRunning = false;;
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	
	//检查 CursorHit.GetActor() 返回的 Actor 是否实现了 IEnemyInterface 接口。
	//如果实现了接口，Cast 返回该对象的接口指针；否则，返回 nullptr。
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * Line trace from cursor. There are several sceneior:
	 * A: LastActor is null && ThisActor is null
	 *  -Do nothing.
	 * B: LastActor is null && ThisActor is valid
	 *	- Highlight ThisActor
	 * C: LastActor is valid && ThisActor is null
	 *	- UnHighlight LastActor
	 * D: Both Actor are valid, LastActor != ThisActor
	 *  - UnHighLight LastActor, Highlight ThisActor
	 * E: Both Actor are valid, LastActor == ThisActor
	 *  - Do nothing
	 */
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//检查是否是鼠标左键点
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//判断ThisActor是否是空指针
		bTargeting = ThisActor ? true : false;
		//轻点鼠标代表不是自动跑路
		bAutoRunning = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//如果按的不是鼠标左键
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//检查是否有合法的ASC
		if (GetASC())
		{
			//释放技能
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	//既不是在瞄准，也没有按下Shift则可以继续自动寻路
	if (!bTargeting && !bShiftKeyDown)
	{
		/*运行到此处说明没有进行Targeting,则可以继续执行移动逻辑
		 * Release后 还要查看是否是Short click
		 */
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressedThreshold && ControlledPawn)
		{
			//寻找路径, 该函数返回一了Navigation Path的指针
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				//每次调用Spline之前要对Spline进行清空
				Spline->ClearSplinePoints();
				//PathPoints返回一个Vector数组， 该数组包含路径中所路过的坐标点
				for (const FVector& PointLoc: NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 3.f);
				}
				//若目标点不在Nav Desh中，则移动到最后可移动到的点
				if (NavPath->PathPoints.Num() > 0) CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() -	1];
				bAutoRunning = true;
			}
		}
		//重制跟随时间和是否瞄准的是角色
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//如果按的不是鼠标左键
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//检查是否有合法的ASC
		if (GetASC())
		{
			//释放技能
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	//如果是在瞄准,则释放技能
	if (bTargeting || bShiftKeyDown)
	{
		//检查是否有合法的ASC
		if (GetASC())
		{
			//释放技能
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		/*运行到这里则是 执行点击移动逻辑*/

		//增加FollowTime
		FollowTime += GetWorld()->GetDeltaSeconds();
		//寻找鼠标点到的位置
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		//获取控制的Pawn
		if (APawn* ControlledPawn = GetPawn())
		{
			//Pawn到目标位置的方向
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

//Controller中获取ASC
UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	
	return AuraAbilitySystemComponent;
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	//获取“增强输入”
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	//添加Context mapping添加到增强输入中
	//Subsystem->AddMappingContext(AuraContext, 0);

	//显示鼠标
	bShowMouseCursor = true;
	//设置默认鼠标光标的样式
	DefaultMouseCursor = EMouseCursor::Default;

	/**
	FInputModeGameAndUI 允许用户既能操作 UI 元素，又能控制游戏行为。
	FInputModeGameOnly：只处理游戏输入（忽略 UI）。
	FInputModeUIOnly：只处理 UI 输入（忽略游戏交互）。
	FInputModeGameAndUI：同时处理游戏和 UI 输入。
	*/
	FInputModeGameAndUI InputModeData;

	//此处使用 DoNotLock，表示鼠标可以自由移动，不会限制在视口范围内。
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	//设置鼠标捕获时是否隐藏光标。
	InputModeData.SetHideCursorDuringCapture(false);

	//将配置好的输入模式应用到玩家控制器。
	SetInputMode(InputModeData);
}

void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	//构造函数式声明rotator,只关注Yaw
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f);

	//从旋转矩阵中获取 Forward vector & Right vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X); 
	}
		
}


