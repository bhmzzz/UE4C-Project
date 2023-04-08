// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class EPlayerACKMode : uint8
{
	ENoneMode,		//未装备武器时
	EMidPoseMode,	//中段攻击模式
	EHighPoseMode,	//上段攻击模式
	EUpperACKMode,
	EDownACKMode,
	EAirACKMode
};

//玩家的残影两种触发类型
UENUM()
enum class EPlayerFlashType : uint8
{
	ENULL,			//无类型
	EClosetFlash,	//极限闪避
	EFlashBody,		//瞬身
	ESlide
};

UENUM(BlueprintType)
enum class ESldeType : uint8
{
	ENULL,
	ESlideF,
	ESlideB,
	ESlideL,
	ESlideR,
	ESlideLF,
	ESlideRF,
	ESlideLB,
	ESlideRB
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	ENULL,	//空
	EJHReady, //居合准备
	ECYZ,	//次元斩
	EJuHe,	//居合
	EJuHe1,
	EJuHe2
};
//攻击到AI的攻击类型用于判断AI该以何种方式受击
UENUM(BlueprintType)
enum class EHitAIACKType : uint8
{
	ComboHit,
	ExcuteHit,
	HitDown,
	HitFly
};

UENUM()
enum class EPlayerHittedType : uint8
{
	CommonHit,
	HitFly
};

USTRUCT(BlueprintType)
struct FPlayerData {
	GENERATED_BODY()
	float HighACKDmg = 5.f;
	float MidACKDmg = 3.f;
	float CYZDmg = 8.f;
	float JuheDmg = 10.f;
	float UpperACKDmg = 8.f;
	float DownACKDmg = 8.f;
	float AirACKDmg = 4.f;
	float OpenFireDmg = 2.f;
	float MissileDmg = 10.f;

	float HP_Heal = 30.f;
	float SP_Heal = 50.f;
	FPlayerData();
};


class ANormalEnemyCharacter;

UCLASS()
class SOULSLIKEDEMO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ANormalEnemyCharacter* EnemyAIIns;

	class AMyGameMode* GameModeIns;
	
	class AMainPlayerController* PCIns;

	class UAnimInstance* AnimInstance;

	float TimeCounter = 0.f; //计时秒表，记录游戏中的时间,并求差值

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* SunGlasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* springArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Blade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* ParticleSystemComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPawnNoiseEmitterComponent* NoiseEmitterComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAIPerceptionStimuliSourceComponent* AIPerStiSourComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* PlayerUiWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* DownACKBox;

	//装备类动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
		UAnimMontage* EquipAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
		UAnimMontage* UnEquipAnim;

	//普通类型攻击动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* PlayerComboACK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* UpperACKMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* AirDownACK_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* MidAckMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* AirComboMontage;
	
	UAnimMontage* CurrentACKMontage;
	UAnimMontage* MidCurrMontage;
	UAnimMontage* HighCurrMotage;
	UAnimMontage* CurrentMakeDamgeMontage;

	//技能型攻击动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* JuHeMontage;

	//玩家休闲动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuMontage")
	UAnimMontage* PlayerSitDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuMontage")
	UAnimMontage* PlayerSitUp;

	//闪避型动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* FlashBodyMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* RollBackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	TMap<ESldeType,UAnimMontage*> SlideMontages;

	//受击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* CommonHittedMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontageToPlay")
	UAnimMontage* HitFlyMontage;


	//与攻击相关的布尔
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "ACKVar")
	bool bIsEquip;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ACKVar")
	bool bIsAttacking;
	//与闪避相关布尔
	bool bSlideTimming;	//滑步闪避触发，用于判断是否极限闪避
	UPROPERTY(BlueprintReadOnly)
	bool bIsSliding;
	bool bIsPerfectSliding;
	UPROPERTY(BlueprintReadOnly)
	bool bHitted;
	bool bCheckDownACK;
	bool bIsPlayACKSound;
	bool bEnergyEmpty;


	FOnMontageEnded MontageEndedDelagate;
	//FOnMontageEnded UpperMontageEndedDelegate;
	
	TArray<FName> SocketNames;
	TArray<FVector> SocketLocation;
	TArray<class ANormalEnemyCharacter*> TargetEnemy;
	TArray<AActor*> UpperACKTargetArr;

	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery> > TargetTypes;

	//定时器相关变量
	FTimerHandle Handler;
	FTimerHandle MidAckHandler;
	FTimerHandle LockHandler;
	FTimerHandle TrailHandler;	//拖尾定时器
	FTimerHandle StrikeHandler; //打击感延时定时器
	FTimerHandle SlideHandler;  //极限闪避时间句柄
	FTimerHandle TimeDilationHandler; //极限闪避时间膨胀句柄
	FTimerHandle SpawnSkillHandler;		//生成技能Actor延时句柄
	FTimerHandle UpperPlayerHandler;	//上挑攻击玩家时间句柄
	FTimerHandle DownACKHandler;	//落地斩计时器，在落地后删除
	FTimerHandle FlashShadowHandler; //残影计时器
	FTimerHandle AutoFireHandler;	//Robot自动开火定时器
	FTimerHandle TakeActionHandler;
	FTimerHandle MontageDelayHandler;
	FTimerHandle SkillPointsHealHandler;
	FTimerHandle SpawnHitSparkHandler;
	FTimerHandle EnergyEmptyTimer;

	//角色状态相关变量
	class AMyPlayerState* PlayerStateTemp;
	EPlayerStateMode PlayerCurrentMode;
	EPlayerStateMode PlayerLockMode;
	EPlayerACKMode CurrPlayerACKMode;
	EPlayerACKMode ACKModeTemp;
	EHitAIACKType HitAIType;

	//锁定系统相关变量
	//TArray<AActor*> TargetActorArray;
	TMap<AActor*, float> TargetActorArray;
	TArray<float> DifZValueArr;
	AActor* FinalLockTarget;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsLock;


	//技能系统相关变量
	ESkillType CurrentSkillType;
	TMap<ESkillType,float> JuHeDis;
	ESkillType JuHeDisType;
	bool bLShiftIsPressed;
	bool bLMouseButtonPressed;
	bool bDownACK;	//下落攻击bool值
	bool bPressF;
	bool bCanCombo;
	bool bHealSkillPointInProgress;
	bool bIsHitted;
	bool PlayerDied;
	bool bIsBackroll;


	//AutoRobot相关变量
	bool bIsOpenFire;
	class AAutoRobot* Robot;

	UPROPERTY()
	float InputX;
	UPROPERTY()
	float InputY;
	UPROPERTY()
	float CurrentX;
	UPROPERTY()
	float CurrentY;

	//当前玩家的残影触发类型
	EPlayerFlashType CurrentFlashType;
	ESldeType CurrentSldeType;


	//玩家属性系统相关变量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "PlayerOption")
		float HP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "PlayerOption")
		float SP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "PlayerOption")
		int SkillPoints;
	FPlayerData* PlayerData;

	//玩家道具变量
	TMap<FString, int> ItemNum;

	FVector HitBackTargetLocation;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void InitPlayer();
	UFUNCTION()
		void MoveForward(float value);
	UFUNCTION()
		void MoveRight(float value);
	UFUNCTION()
		void Turn(float value);
	UFUNCTION()
		void LookUp(float value);
	UFUNCTION()
		void Run();
	UFUNCTION()
		void StopRun();
	UFUNCTION()
		void MyJump();
	UFUNCTION()
		void MyStopJump();
	UFUNCTION()
		void PlayerCombo();
	UFUNCTION()
		void SetCheckCombo();
	UFUNCTION()
		void ReleasedLMouseB();
	UFUNCTION()
		void EquipWeapon();
	UFUNCTION()
		void UnEquipWeapon();
	UFUNCTION()
		UStaticMeshComponent* GetWeapon();
	UFUNCTION()
		void CheckPlayerAttack();
	UFUNCTION()
		void EquipEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void UnEquipEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void AttackEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void ChangeCombatSection();
	UFUNCTION()
		void InitSocketLocation();
	UFUNCTION()
		void AddTargetEnemy(TArray<FHitResult> OutHits);
	UFUNCTION()
		void MontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void MontageNotifyEnded(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void CheckMode(EPlayerStateMode& playerMode);
	UFUNCTION()
		void LockSystem();
	UFUNCTION()
		void FindTargetEnemy();
	UFUNCTION()
		void LockLastCheck();
	UFUNCTION()
		void MakeLockRotation();
	UFUNCTION()//获取到被攻击的敌人数组
		TArray<ANormalEnemyCharacter*> GetTargetEnemy();
	UFUNCTION()
		void ShowTrail();
	UFUNCTION()
		void EndTrail();
	UFUNCTION()
		void Set_NoneMode();
	UFUNCTION()
		void Set_MidACKMode();
	UFUNCTION()
		void Set_HighACKMode();
	UFUNCTION()
		void Switch_ACKPose();
	UFUNCTION()
		void Show_ACKPose();
	UFUNCTION()
		void CombatStrikeSet(UAnimMontage* CurrentCombatMonateg);
	UFUNCTION()
		void StrikeTimeEnd();
	UFUNCTION()
		void SlideTo();
	UFUNCTION()
		void SlideTimeOut();
	UFUNCTION()
		void CheckPerfectSlide();
	UFUNCTION()
		void TimeDilationOut();
	UFUNCTION()
		float GetInputX();
	UFUNCTION()
		float GetInputY();
	UFUNCTION()
		void JuHeSkill();
	UFUNCTION()
		void SpawnJuHeEffect();
	UFUNCTION(BlueprintCallable)
		AActor* GetLockTarget();
	UFUNCTION()
		void ACKUpper();
	UFUNCTION()
		bool CheckUpperAI();
	UFUNCTION()
		void UpperPlayer();
	UFUNCTION()
		void SetDownACK();
	UFUNCTION()
		bool GetDownACKBool();
	UFUNCTION()
		void DownACKTimer();
	UFUNCTION()
		void DownACKChecker();
	UFUNCTION()
		void UpperMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		TArray<AActor*> GetUpperTarget();
	UFUNCTION()
		bool PlayerIsInAir();
	//此函数可用于所有攻击蒙太奇数组
	UFUNCTION()
		void PlayNextMontage(const TArray<UAnimMontage*>& MontageArray, UAnimMontage* CurrentAnimMontage);
	//瞬身/闪现
	UFUNCTION()
		void FlashPlayer();
	UFUNCTION()
		void AirFlash();
	UFUNCTION()
		void AirFlashSapwnShadow();
	UFUNCTION()
		void RollBackEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void SetPlayerMoveLoc();
	UFUNCTION()
		void SetPlayerFlashLocation();
	UFUNCTION()
		void ClearFlashLocationEndHandler();
	//在空中时关闭摄像机延迟
	UFUNCTION()
		void SetCameraDelay();
	UFUNCTION()
		void SpawnShadowActor();
	UFUNCTION()
		EPlayerFlashType GetPlayerFlashType();

	UFUNCTION()
		void SpawnFollowRobot();

	//这个接口用于机器人判断当前玩家状态以设置上下悬浮的开启与关闭
	UFUNCTION()
	bool IsMoveOrJump();

	UFUNCTION()
		void RobotOpenFire();
	UFUNCTION()
		void BackCallOpenFire();
	UFUNCTION()
		void RobotStopFire();
	UFUNCTION()
		bool IsRobotOpenFire();
	UFUNCTION()
		bool IsLock();
	//当锁定目标太远时解除锁定。
	UFUNCTION()
		void UnlockWhenTooFar();
	UFUNCTION()
		void Unlock(ANormalEnemyCharacter* EnemyTemp);
	UFUNCTION(BlueprintCallable)
		AAutoRobot* GetRobot();
	UFUNCTION()
		void SetPlayerUIRotation();
	UFUNCTION()
		float GetHP();
	UFUNCTION()
		float GetSP();
	UFUNCTION(BlueprintCallable)
		bool GetPressF();
	UFUNCTION()
		void TakeAction();
	UFUNCTION()
		void SetTakeAction();
	UFUNCTION()
		void OverShoulderView();
	UFUNCTION()
		void ResetCameraLocation();
	UFUNCTION()
		void PlaySitUp();
	UFUNCTION()
		void SetNormalInputMode();

	UFUNCTION()
		void InitlizeItemNums();
	UFUNCTION()
		TMap<FString, int>& GetItemNum();
	UFUNCTION()
		void SwitchMagicItem();
	UFUNCTION()
		void SwitchMedcineItem();

	//用于查找距离玩家的任意Actor的距离
	UFUNCTION()
		float GetDisWithActor(AActor* Target);
	UFUNCTION()
		float GetDisWithVector(FVector TargetLocation);
	UFUNCTION()
		void SetPlayerACK_MoveLocation();
	UFUNCTION()
		void KeysNeedToToggleOrSet(TArray<FKey>& Keys, bool bIngore);

	UFUNCTION()
		void SlideMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void DisplayPlayerMode();
	UFUNCTION()
		void PlayHittedMontage(EPlayerHittedType Type);
	UFUNCTION()
		void SetPlayerHittedBack();
	UFUNCTION()
		void HittedMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void PushPlayerToAI();
	UFUNCTION()
		void SetSafeDisWithAI();
	UFUNCTION()
		bool SkillReadyProgress();
	//使用技能点和恢复技能点
	UFUNCTION(BlueprintCallable)
		void UseSkillPoints();
	UFUNCTION()
		void HealSkillPoints();
	UFUNCTION()
		void HealSkillPointsDelegate();
	UFUNCTION()
		void TakeHealth(float DamageValue);
	UFUNCTION()
		void TakeEnergy(float CostValue);
	UFUNCTION()
		void HealEnergy();
	UFUNCTION()
		void ClearHealHandler();
	UFUNCTION()
		void DownACKCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void StopSlideForward();
	UFUNCTION(BlueprintImplementableEvent)
		void SetCameraShake();
	UFUNCTION()
		void ACKDelay();
	UFUNCTION()
		void ACKDelayOver();
	UFUNCTION()
		bool GetPlayACKSoundBoolean();
	UFUNCTION()
		void PlaySwingSound();
	UFUNCTION()
		void PlayHitSpark(FVector HitLocation);
	UFUNCTION()
		void HitSparkOver(UParticleSystemComponent* system);
	UFUNCTION()
		void FireHealPlayerItem();
	UFUNCTION()
		void UseItem();
	//对AI造成伤害
	UFUNCTION()
		void MakeDamage();
	void SaveMyGame();





};
