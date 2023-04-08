// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NormalEnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class ENormalEnemyMode : uint8 {
	ENormal,	//没有发现玩家，处于正常状态
	ELockPlayer, //发现玩家，进入追逐与攻击状态
	EAttack		//攻击玩家
};

UENUM(BlueprintType)
enum class EDistanceType : uint8 {
	EFar,		//远距离
	EMid,		//中距离
	EMTN,
	ENear,		//近距离
	EClose		//近身
};

UENUM(BlueprintType)
enum class EHittedType : uint8 {
	None,		//未受击
	Excute,		//处决受击
	ComboHit,	//普通连击受击
	HitDown,	//击倒受击
	HitFly		//击飞受击
};

UENUM()
//对玩家造成的hit类型
enum class EMakeHitType : uint8
{
	CommonHit,
	HitFly
};

UENUM()
//Boss的属性状态
enum class EBossStateType : uint8
{
	NONE,
	BT,
	QJ,
	LJ,
	RS,
	DIE
};

USTRUCT(BlueprintType)
struct FBossDamage {
	GENERATED_BODY()
	float LightingDmg = 20.f;
	float JumpACKDmg = 20.f;
	float JuheDmg = 20.f;
	float ComboDmg = 20.f;
	float BladeLightDmg = 20.f;
	FBossDamage();
};

UCLASS()
class SOULSLIKEDEMO_API ANormalEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalEnemyCharacter();

	bool bCurrMontagePlay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	class APlayerCharacter* player;
	class ANormalEnemyController* AIC;
	class UAnimInstance* AnimInstance;

	/*UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	class UBoxComponent* CheckCollision;*/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* Hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LockMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;
	//定义角色感知组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPawnSensingComponent* PawnSensingComp; 




	//蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HittedMontage")
		class UAnimMontage* EnemyHitedMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HittedMontage")
		class UAnimMontage* ExcuteMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HittedMontage")
		class UAnimMontage* HitFlyMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HittedMontage")
		class UAnimMontage* HitDownMontage;
		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
		class UAnimMontage* EnemyCombatMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
		class UAnimMontage* JumpACKMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
		class UAnimMontage* DogeBackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
		class UAnimMontage* JumpAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
		class UAnimMontage* BladeLightAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
		class UAnimMontage* BossLossMontage;
	UAnimMontage* CurrentMontage;

	class UAnimMontage* CurrentACKMontage;

	FOnMontageEnded MontageEndedDelegate;

	FScriptDelegate HearDelegate;//AI听觉执行函数的代理

	float TimeCounter = 0.f;

	////角色变量
	//UPROPERTY()
	//	EPlayerCurDirection playerCurrentDirect;
	UPROPERTY()
		class UEnemyAnimInstance* AnimInstanceTemp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		bool bBeHitted;

	//定时器相关变量
	FTimerHandle LightingACKHandler;
	FTimerHandle IsLightingACKHandler;
	FTimerHandle HitBackHandler;
	FTimerHandle CombatHandler;
	FTimerHandle SprintACKHandler;
	FTimerHandle UpperHandler; //AI被上挑时间定时器
	FTimerHandle DelayHandler; //延迟定时器
	FTimerHandle SlowMonatgeHandler; //极限闪避触发handler
	FTimerHandle BTTDelayHandler; //BTT定时器，用于处理各种任务播放动画完成判断
	FTimerHandle EnergyEmptyHandler;
	FTimerHandle BurnCounterHandler;
	FTimerHandle BurnningHandler;
	FTimerHandle LightingCounterHandler;
	FTimerHandle LightingHandler;
	FTimerHandle SwitchHittedHandler;
	bool bAnimDelayComplete;

	//攻击检测相关变量
	TArray<AActor*> TargetArr;
	TArray<FName> SocketNameArr;
	TArray<FVector> SocketLocationArr;
	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery> > TargetTypes;

	//AI当前行为模式变量（枚举）
	ENormalEnemyMode EnemyCurrentMode;
	EHittedType AiHittedType;
	FVector PlayerCurrentLocation;	//在需要全局调用玩家当前时刻位置变量
	EMakeHitType MakeHitType;
	EBossStateType BodyState;
	EBossStateType DeBuffState;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "DistanceType")
	EDistanceType CurrentDistance;

	UPROPERTY()
	class APlayerCharacter* PlayerTemp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* CollisionBox;

	//AI属性数值相关变量
	float HP;
	float Armmor;
	TSharedPtr<class SEnemyWidget> EnemyWidget;

	bool bIsSlowAction;
	bool bArmmorEmpty;
	bool bIsBurnning;
	bool bIsLighting;
	bool bDied;
	bool bIsFocus;
	bool bIsLightingACK;
	bool bIsPlayHittedMontage;
	bool bHitAgain;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void SeePlayer();
	

	UFUNCTION()
		void SetLockMeshVisible();
	UFUNCTION()
		UStaticMeshComponent* GetLockMesh();
	UFUNCTION()
		void GetDIfRotation();
	UFUNCTION(BlueprintCallable)
		void SetHittedValue(bool value);
	UFUNCTION()
		void PlayHittedMontage(UAnimMontage* CurrentHittedMontage);
	UFUNCTION()
		void WhenNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void WhenNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void MakeHitBack();
	UFUNCTION()
		void PlayComboMontage();
	UFUNCTION()
		void HittedMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		bool GetHittedValue();
	UFUNCTION()
		void CheckACK();
	UFUNCTION()
		void InitSocketLocation();
	UFUNCTION()
		void AddTarget(TArray<FHitResult>& HitResult);
	UFUNCTION()
		EDistanceType& GetCurrDistanceType();
	UFUNCTION()
		void PrintDistance();
	UFUNCTION()
		void PlaySprintACKMontage();
	UFUNCTION()
		void SprintMovement();
	UFUNCTION()
		void UpperAI();
	UFUNCTION()
		void UpperEndAI();
	UFUNCTION()
		bool AIIsInAir();
	UFUNCTION()
		void CancelDelayHandler();
	UFUNCTION()
		void BeAirCombEffect();
	UFUNCTION()
		void AirHittedReSetLocation(float value);
	//这个函数接口可用于一切box检测玩家，可自设置box大小
	UFUNCTION()
		bool BoxTraceCheckPlayer(float value,FVector HalfSize);
	UFUNCTION(BlueprintImplementableEvent)
		void ActivateNiagaraComp();
	UFUNCTION(BlueprintImplementableEvent)
		void DeActivateNiagaraComp();
	UFUNCTION()
		void SpawnLightingCheckBox();

	UFUNCTION() //一定要加，否则无效
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,    //触发时间的主体，通常是控制的人
			UPrimitiveComponent* OterComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void AIDogeBack();
	UFUNCTION()
		bool BTTDelayCheck();
	UFUNCTION()
		void SingleACKMontageEnd(class UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
		void AIJumpAttack();
	UFUNCTION()
		void JumpAttackMovement();
	UFUNCTION()
		void SpawnJumpAttackEffect();

	UFUNCTION()
		void AIBladeLightAttack();
	//对该AI造成伤害接口
	UFUNCTION(BlueprintCallable)
		void TakeDamageAndAmmor(float DamageValue);
	UFUNCTION()
		void SetHittedType(FString HitType = "");
	UFUNCTION()
		UAnimMontage* GetHittedMontage();
	UFUNCTION()
		float GetDisWithVector(FVector& TargetLocation);
	UFUNCTION()
		void SlowEnemyAction();
	UFUNCTION()
		void SlowActionOver();
	UFUNCTION()
		void DisplayDIstanceType();
	UFUNCTION()
		void AIHitFly();
	UFUNCTION()
		void CannelEnergyEmpty();
	UFUNCTION()
		void HealEnergyValue();
	UFUNCTION()
		void SetBurnEffect();
	UFUNCTION()
		void IsBurning();
	UFUNCTION()
		void BurnOver(AMainHUD* Hud);
	UFUNCTION()
		void SetLightingEffect();
	UFUNCTION()
		void IsLighting();
	UFUNCTION()
	void LightingOver(AMainHUD* Hud);
	UFUNCTION()
	void CurrMontageOver(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void LightingACK(ALightingCheckActor* actor);
	UFUNCTION()
		void LightingACKOver(class ALightingCheckActor* actor);
	UFUNCTION()
		void BossDied();
	UFUNCTION()
		void SetFocusBoolean(bool value);
	UFUNCTION()
		void DisplayFoucus();
	UFUNCTION()
		void DisplayHitted();
	UFUNCTION()
		void DisplayState();
	UFUNCTION()
		EBossStateType GetBodyState();
	UFUNCTION()
		bool GetLightingACKBoolean();
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnBladeLight();
	UFUNCTION()
		void SetPlayerHittedMontage(EMakeHitType HitType);
	UFUNCTION()
		void SetInplayHittedMontageBoolean(bool value);
	UFUNCTION()
		bool GetplayHittedMontageBoolean();
	UFUNCTION()
		void HittedValueDelay();
	UFUNCTION()
		void OldSetHittedValue();
	UFUNCTION()
		bool GetHitAgainBoolean();

};
