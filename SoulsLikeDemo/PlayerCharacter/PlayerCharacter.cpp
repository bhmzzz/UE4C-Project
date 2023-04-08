// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SoulsLikeDemo/Enemy/NormalEnemyCharacter.h"
#include "MyPlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "particles/ParticleSystemComponent.h"
#include "SoulsLikeDemo/SkillActor/TestSkillActor.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "SoulsLikeDemo/SkillActor/FlashBodyEffect.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "SoulsLikeDemo/Robot/AutoRobot.h"
#include "SoulsLikeDemo/Robot/MissileActor.h"
#include "Components/WidgetComponent.h"
#include "SoulsLikeDemo/Widget/SPlayerWidget.h"
#include "SoulsLikeDemo/MyStruct/MainPlayerController.h"
#include "SoulsLikeDemo/MyStruct/MyGameMode.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "SoulsLikeDemo/SaveGame/MySaveGame.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitPlayer();//初始化玩家

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	GameModeIns = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PCIns = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AnimInstance = GetMesh()->GetAnimInstance();
	EnemyAIIns = Cast<ANormalEnemyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANormalEnemyCharacter::StaticClass()));

	//动态加载
	/*FSoftClassPath PlayerUIRef(TEXT("/Game/UI/PlayerWidgetNew"));
	TSubclassOf<UUserWidget> WidgetClass = PlayerUIRef.TryLoadClass<UUserWidget>();
	if (WidgetClass && PlayerUiWidget)
		PlayerUiWidget->SetWidgetClass(WidgetClass);*/

	//在BeginPlay()时生成跟随机器人。
	SpawnFollowRobot();

	Super::BeginPlay();
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &APlayerCharacter::MontageNotifyBegin);
	AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &APlayerCharacter::MontageNotifyEnded);
	DownACKBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::DownACKCheck);
	PlayerStateTemp = Cast<AMyPlayerState>(GetPlayerState());
	Set_NoneMode();

	InputX = 0.0f;
	InputY = 0.0f;

	//开始游戏时玩家播放围坐篝火动画
	if (PlayerSitDown)
	{
		AnimInstance->Montage_Play(PlayerSitDown);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CheckPlayerAttack();
	//CheckMode(PlayerCurrentMode);
	//Show_ACKPose();
	//PlayerIsInAir();
	SetCameraDelay();
	UnlockWhenTooFar();
	SetPlayerUIRotation();
	//DisplayPlayerMode();
	SetSafeDisWithAI();
	HealSkillPoints();
	HealEnergy();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::StopRun);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::MyJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::MyStopJump);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::PlayerCombo);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacter::ReleasedLMouseB);
	PlayerInputComponent->BindAction("UnEquip", IE_Pressed, this, &APlayerCharacter::UnEquipWeapon);
	PlayerInputComponent->BindAction("Lock", IE_Pressed, this, &APlayerCharacter::LockSystem);
	PlayerInputComponent->BindAction("SwitchACKPose", IE_Pressed, this, &APlayerCharacter::Switch_ACKPose);
	PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &APlayerCharacter::SlideTo);
	PlayerInputComponent->BindAction("RobotOpenFire", IE_Pressed, this, &APlayerCharacter::RobotOpenFire);
	PlayerInputComponent->BindAction("RobotOpenFire", IE_Released, this, &APlayerCharacter::RobotStopFire);
	PlayerInputComponent->BindAction("TakeAction", IE_Pressed, this, &APlayerCharacter::TakeAction);
	
	PlayerInputComponent->BindAction("SwitchIMagictem", IE_Pressed, this, &APlayerCharacter::SwitchMagicItem);
	PlayerInputComponent->BindAction("SwitchIMedcinetem", IE_Pressed, this, &APlayerCharacter::SwitchMedcineItem);
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &APlayerCharacter::UseItem);

}

void APlayerCharacter::InitPlayer()
{
	SunGlasses = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SunGlasses"));
	SunGlasses->SetupAttachment(GetMesh(), FName("SunGlassesSocket"));
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	springArm->SetupAttachment(GetRootComponent());
	camera->SetupAttachment(springArm);
	springArm->SetRelativeLocation(FVector(0.f,0.f,80.f));

	PlayerUiWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerUiWidget"));
	PlayerUiWidget->SetupAttachment(GetMesh());
	PlayerUiWidget->SetRelativeLocation(FVector(0.00062, 100.0, 70.0));
	PlayerUiWidget->SetRelativeRotation(FRotator(0, 0, -180));
	PlayerUiWidget->SetRelativeScale3D(FVector(1, 0.3, 0.3));
	PlayerUiWidget->SetPivot(FVector2D(0.5, 0.5));

	springArm->TargetArmLength = 300.f;//设置弹簧臂的长度为300
	springArm->bUsePawnControlRotation = true;//设置使用角色控制旋转
	springArm->bEnableCameraLag = true;//开启弹簧臂延迟
	springArm->CameraLagSpeed = 3;//设置延迟速度为3

	camera->bUsePawnControlRotation = false;//关闭摄像机的跟随旋转

	//保证控制器不会随着视角反转
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);//设置旋转速率
	GetCharacterMovement()->JumpZVelocity = 600.0f;//设置跳跃速度
	GetCharacterMovement()->AirControl = 0.2f;

	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	Blade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blade"));
	Blade->SetupAttachment(GetMesh(), FName("EquipPlace"));
	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	ParticleSystemComp->SetupAttachment(Blade);
	DownACKBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DownACKBox"));
	DownACKBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	DownACKBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	DownACKBox->SetupAttachment(Blade);
	DownACKBox->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	DownACKBox->bHiddenInGame = true;

	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComp"));
	AIPerStiSourComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerStiSourComp"));

	CurrentSkillType = ESkillType::ENULL;
	CurrentFlashType = EPlayerFlashType::ENULL;
	CurrentSldeType = ESldeType::ENULL;
	PlayerCurrentMode = EPlayerStateMode::ENORMAL;
	//默认锁定模式为装备武器模式，该变量只需要装备模式或者锁定模式利于判断是否处于锁定状态
	PlayerLockMode = EPlayerStateMode::EEQUIP;
	ACKModeTemp = EPlayerACKMode::EHighPoseMode;
	JuHeDis.Add(ESkillType::EJuHe1, 300.f);
	JuHeDis.Add(ESkillType::EJuHe2, 500.f);
	JuHeDisType = ESkillType::ENULL;
	HitAIType = EHitAIACKType::ComboHit;
	bIsEquip = false;

	CurrentACKMontage = nullptr;

	HP = 100;
	SP = 100;
	SkillPoints = 5;
	//初始化玩家的所有道具个数
	InitlizeItemNums();
	//初始化
	PlayerData = new FPlayerData();

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerSitDownMontage(TEXT("/Game/K_Animation/PlayerSitDown"));
	if (PlayerSitDownMontage.Succeeded())
	{
		PlayerSitDown = PlayerSitDownMontage.Object;
	}
}

void APlayerCharacter::MoveForward(float value)
{
	InputY = value;
	//在玩家处于瞬身，垫步，攻击,受击不能移动
	if (CurrentFlashType == EPlayerFlashType::ENULL && PlayerCurrentMode != EPlayerStateMode::EACK && !PlayerIsInAir() && !bHitted
		&& !SkillReadyProgress())
	{
		if (Controller != nullptr && value != 0.0f)
		{
			const FRotator ConRotator = Controller->GetControlRotation();
			const FRotator YawRotation(0, ConRotator.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, value);
		}
	}
}

void APlayerCharacter::MoveRight(float value)
{
	InputX = value;
	if (CurrentFlashType == EPlayerFlashType::ENULL && PlayerCurrentMode != EPlayerStateMode::EACK && !PlayerIsInAir() && !bHitted
		&& !SkillReadyProgress())
	{
		if (Controller != nullptr && value != 0.0f)
		{
			const FRotator ConRotator = Controller->GetControlRotation();
			const FRotator YawRotation(0, ConRotator.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, value);
		}
	}
}

void APlayerCharacter::Turn(float value)
{
	//在锁定时不能转动视角解决相机抖动问题
	if(!IsLock())
		AddControllerYawInput(value);
}

void APlayerCharacter::LookUp(float value)
{
	if (!IsLock())
		AddControllerPitchInput(value);
}

void APlayerCharacter::Run()
{
	bLShiftIsPressed = true;
	if(PlayerCurrentMode == EPlayerStateMode::ENORMAL)
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	else if (PlayerCurrentMode == EPlayerStateMode::EEQUIP)
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void APlayerCharacter::StopRun()
{
	bLShiftIsPressed = false;
	if (PlayerCurrentMode == EPlayerStateMode::ENORMAL)
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	else if(PlayerCurrentMode == EPlayerStateMode::EEQUIP)
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void APlayerCharacter::MyJump()
{
	if (PlayerCurrentMode == EPlayerStateMode::EEQUIP && PlayerLockMode != EPlayerStateMode::ELOCK)
	{
		Jump();
		PlayerStateTemp->Set_JumpMode(PlayerCurrentMode);
	}
	else if (PlayerLockMode == EPlayerStateMode::ELOCK)//瞬身技能
		FlashPlayer();
}
void APlayerCharacter::MyStopJump()
{
	if (PlayerCurrentMode == EPlayerStateMode::EEQUIP || PlayerCurrentMode == EPlayerStateMode::EJUMP)
	{
		StopJumping();
		PlayerStateTemp->Set_EquipMode(PlayerCurrentMode);
	}
}

void APlayerCharacter::PlayerCombo()
{
	bLMouseButtonPressed = true;
	if (!bIsEquip)
	{
		EquipWeapon();
		return;
	}
	InitSocketLocation();
	//次元斩
	if (PlayerLockMode == EPlayerStateMode::ELOCK && bLShiftIsPressed && InputY == 1.0f)//当处于锁定状态并且按下了左Shift与W键
	{
		//如果正在某种攻击状态那么我们不能将其用新的攻击打断
		if (PlayerCurrentMode == EPlayerStateMode::EACK)
		{
			if (AnimInstance->Montage_IsPlaying(UpperACKMontage))
				AnimInstance->Montage_Stop(0.1f, UpperACKMontage);
			else
				return;
		}
		PlayerStateTemp->Set_ACKMode(PlayerCurrentMode);
		GetWorldTimerManager().ClearTimer(TakeActionHandler);
		GetWorldTimerManager().ClearTimer(SpawnSkillHandler);
		GetWorldTimerManager().ClearTimer(FlashShadowHandler);
		//判断1.5秒长按键,触发次元斩或者带位移的居合斩
		if (TimeCounter != 0)
			TimeCounter = 0;
		MontageEndedDelagate.Unbind();
		MontageEndedDelagate.BindUObject(this, &APlayerCharacter::AttackEnded);
		AnimInstance->Montage_Play(JuHeMontage);
		CurrentMakeDamgeMontage = JuHeMontage;
		UseSkillPoints();
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate);
		CurrentSkillType = ESkillType::EJHReady;
		GetWorldTimerManager().SetTimer(TakeActionHandler, this, &APlayerCharacter::JuHeSkill, GetWorld()->DeltaTimeSeconds, true);
	}
	//上挑攻击
	else if (PlayerLockMode == EPlayerStateMode::ELOCK && bLShiftIsPressed && InputY == -1.f && !PlayerIsInAir() && !bEnergyEmpty)//判断上挑攻击释放条件
	{
		if (PlayerCurrentMode == EPlayerStateMode::EACK && !AnimInstance->Montage_IsPlaying(MidAckMontage)
			&& !AnimInstance->Montage_IsPlaying(PlayerComboACK))
			return;

		if (AnimInstance->Montage_IsPlaying(MidAckMontage))
			AnimInstance->Montage_Stop(0.1f, MidAckMontage);
		else if(AnimInstance->Montage_IsPlaying(PlayerComboACK))
			AnimInstance->Montage_Stop(0.1f, PlayerComboACK);
		//如果正在某种攻击状态(除了普通攻击)那么我们不能将其用新的攻击打断
		PlayerStateTemp->Set_ACKMode(PlayerCurrentMode);
		CurrPlayerACKMode = EPlayerACKMode::EUpperACKMode;
		UseSkillPoints();
		TakeEnergy(15.f);
		ACKUpper();
	}
	//下落攻击
	else if (bLShiftIsPressed && InputY == -1.f && PlayerIsInAir() && !bEnergyEmpty)
	{
		
		float dis = GetDisWithVector(FVector(GetActorLocation().X, GetActorLocation().Y, 0.f)) - 90.f;
		if (dis >= 50.f)
		{
			//如果正在某种攻击状态那么我们不能将其用新的攻击打断
			if (PlayerCurrentMode == EPlayerStateMode::EACK)
			{
				if (AnimInstance->Montage_IsPlaying(UpperACKMontage))
					AnimInstance->Montage_Stop(0.f, UpperACKMontage);
				else if(AnimInstance->Montage_IsPlaying(AirComboMontage))
					AnimInstance->Montage_Stop(0.f, AirComboMontage);
				else
					return;
			}
			if (AnimInstance && AirDownACK_Montage)
			{
				PlayerStateTemp->Set_ACKMode(PlayerCurrentMode);
				CurrPlayerACKMode = EPlayerACKMode::EDownACKMode;
				MontageEndedDelagate.Unbind();
				MontageEndedDelagate.BindUObject(this, &APlayerCharacter::AttackEnded);
				AnimInstance->Montage_Play(AirDownACK_Montage);
				UseSkillPoints();
				HitAIType = EHitAIACKType::HitFly;
				bIsAttacking = true;
				TakeEnergy(15.f);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate);
				GetWorldTimerManager().SetTimer(DownACKHandler, this, &APlayerCharacter::DownACKChecker, GetWorld()->DeltaTimeSeconds, true);
			}
		}
	}
	//空中连击
	else if (PlayerIsInAir())
	{
		//首先检测玩家是否距离地面太近，如果太近则不能进行空中攻击
		if ((GetDisWithVector(FVector(GetActorLocation().X, GetActorLocation().Y, 0.f)) - 90.f) >= 50.f && !bEnergyEmpty)
		{
			if (!AnimInstance->Montage_IsPlaying(AirComboMontage))
			{
				TargetEnemy.Empty();
				if (PlayerCurrentMode == EPlayerStateMode::EACK && AnimInstance->Montage_IsPlaying(UpperACKMontage))
					AnimInstance->Montage_Stop(0.f, UpperACKMontage);

					if (AnimInstance && AirComboMontage)
					{
						PlayerStateTemp->Set_ACKMode(PlayerCurrentMode);
							CurrPlayerACKMode = EPlayerACKMode::EAirACKMode;
							MontageEndedDelagate.BindUObject(this, &APlayerCharacter::AttackEnded);
							AnimInstance->Montage_Play(AirComboMontage);
							CurrentMakeDamgeMontage = AirComboMontage;
							HitAIType = EHitAIACKType::ComboHit;
						AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate);
						bIsAttacking = true;
						TakeEnergy(15.f);
					}
			}
			else if (AnimInstance && AirComboMontage && AnimInstance->Montage_IsPlaying(AirComboMontage) && !bEnergyEmpty && bCanCombo)
			{
				TargetEnemy.Empty();
				TakeEnergy(15.f);
				PlayerStateTemp->Set_ACKMode(PlayerCurrentMode);
				ChangeCombatSection();
			}
		}
		else if ((GetDisWithVector(FVector(GetActorLocation().X, GetActorLocation().Y, 0.f)) - 90.f) < 50.f)
		{
			TargetEnemy.Empty();
			if (AnimInstance && AnimInstance->Montage_IsPlaying(AirComboMontage))
			{
				AnimInstance->Montage_Stop(0.1f, AirComboMontage);
				CurrPlayerACKMode = ACKModeTemp;
			}
		}
		else
			return;
	}
	//普通攻击（上段、中段）
	else if(!PlayerIsInAir())
	{
		//上段攻击
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(PlayerComboACK) && CurrPlayerACKMode == EPlayerACKMode::EHighPoseMode && !bEnergyEmpty)
		{
			TargetEnemy.Empty();
			//如果此时玩家处于完美闪避状态那么此时普通攻击产生吸附
			if (bIsPerfectSliding)
			{
				GetWorldTimerManager().ClearTimer(StrikeHandler);
				GetWorldTimerManager().SetTimer(StrikeHandler, this, &APlayerCharacter::PushPlayerToAI, GetWorld()->DeltaTimeSeconds, true);
			}
			PlayerStateTemp->Set_ACKMode(PlayerCurrentMode);
			MontageEndedDelagate.BindUObject(this, &APlayerCharacter::AttackEnded);
			AnimInstance->Montage_Play(PlayerComboACK);
			CurrentMakeDamgeMontage = PlayerComboACK;
			CurrentACKMontage = PlayerComboACK;
			HitAIType = EHitAIACKType::ComboHit;
			HighCurrMotage = PlayerComboACK;
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate);
			bIsAttacking = true;
			TakeEnergy(15.f);
		}
		//中段攻击
		else if (AnimInstance && !AnimInstance->Montage_IsPlaying(MidAckMontage) && CurrPlayerACKMode == EPlayerACKMode::EMidPoseMode && !bEnergyEmpty)
		{
			TargetEnemy.Empty();
			//如果此时玩家处于完美闪避状态那么此时普通攻击产生吸附
			if (bIsPerfectSliding)
			{
				GetWorldTimerManager().ClearTimer(StrikeHandler);
				GetWorldTimerManager().SetTimer(StrikeHandler, this, &APlayerCharacter::PushPlayerToAI, GetWorld()->DeltaTimeSeconds, true);
			}
			PlayerStateTemp->Set_ACKMode(PlayerCurrentMode);
			MontageEndedDelagate.BindUObject(this, &APlayerCharacter::AttackEnded);
			AnimInstance->Montage_Play(MidAckMontage);
			CurrentMakeDamgeMontage = MidAckMontage;
			CurrentACKMontage = MidAckMontage;
			HitAIType = EHitAIACKType::ComboHit;
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate);
			MidCurrMontage = MidAckMontage;
			bIsAttacking = true;
			TakeEnergy(10.f);
		}
		else if (AnimInstance && bIsAttacking && (CurrPlayerACKMode == EPlayerACKMode::EHighPoseMode || CurrPlayerACKMode == EPlayerACKMode::EMidPoseMode) && bCanCombo && !bEnergyEmpty)
		{
			PlayerStateTemp->Set_ACKMode(PlayerCurrentMode);
			ChangeCombatSection();
		}
	}
}

void APlayerCharacter::ReleasedLMouseB()
{
	bLMouseButtonPressed = false;
}

void APlayerCharacter::ChangeCombatSection()
{
	if (AnimInstance->Montage_IsPlaying(PlayerComboACK) && CurrPlayerACKMode == EPlayerACKMode::EHighPoseMode)
	{
		TakeEnergy(15.f);
	FName CurrentCombatSection = AnimInstance->Montage_GetCurrentSection();
	if (CurrentCombatSection == "Attack1")
		AnimInstance->Montage_JumpToSection(FName("Attack2"), PlayerComboACK);
	else if (CurrentCombatSection == "Attack2")
		AnimInstance->Montage_JumpToSection(FName("Attack3"), PlayerComboACK);
	else if (CurrentCombatSection == "Attack3")
		AnimInstance->Montage_JumpToSection(FName("Attack4"), PlayerComboACK);
	else if (CurrentCombatSection == "Wait1")
		AnimInstance->Montage_SetNextSection(CurrentCombatSection, FName("End"));
	else if (CurrentCombatSection == "Wait2")
		AnimInstance->Montage_SetNextSection(CurrentCombatSection, FName("End"));
	else if (CurrentCombatSection == "Wait3")
		AnimInstance->Montage_SetNextSection(CurrentCombatSection, FName("End"));
	}
	else if (AnimInstance->Montage_IsPlaying(MidAckMontage) && CurrPlayerACKMode == EPlayerACKMode::EMidPoseMode)
	{
		TakeEnergy(10.f);
		FName CurrentCombatSection = AnimInstance->Montage_GetCurrentSection();
		if (CurrentCombatSection == "MCombat1S")
			AnimInstance->Montage_JumpToSection(FName("MCombat2S"), MidAckMontage);
		else if (CurrentCombatSection == "MCombat2S")
			AnimInstance->Montage_JumpToSection(FName("MCombat3S"), MidAckMontage);
		else if (CurrentCombatSection == "MCombat3S")
			AnimInstance->Montage_JumpToSection(FName("MCombat4S"), MidAckMontage);
		else if (CurrentCombatSection == "MCombat1E")
			AnimInstance->Montage_SetNextSection(CurrentCombatSection, FName("End"));
		else if (CurrentCombatSection == "MCombat2E")
			AnimInstance->Montage_SetNextSection(CurrentCombatSection, FName("End"));
		else if (CurrentCombatSection == "MCombat3E")
			AnimInstance->Montage_SetNextSection(CurrentCombatSection, FName("End"));
	}
	else if (AnimInstance->Montage_IsPlaying(AirComboMontage))
	{
		FName CurrentCombatSection = AnimInstance->Montage_GetCurrentSection();
		if(CurrentCombatSection == "AirACK1S")
			AnimInstance->Montage_JumpToSection(FName("AirACK1S"), AirComboMontage);
		else if(CurrentCombatSection == "AirACK1E")
			AnimInstance->Montage_SetNextSection(CurrentCombatSection, FName("End"));
	}
}

void APlayerCharacter::EquipWeapon()
{
	if (!bIsEquip)
	{
		UWorld* world = GetWorld();
		DisableInput(UGameplayStatics::GetPlayerController(world,0));
		AnimInstance->Montage_Play(EquipAnim);
		MontageEndedDelagate.BindUObject(this, &APlayerCharacter::EquipEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate);
		bIsEquip = true;
		PlayerStateTemp->Set_EquipMode(PlayerCurrentMode);
		Set_HighACKMode();
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;

		bLMouseButtonPressed = false;	//这里必须手动设置不然一直是true，应该是bug
	}
}

void APlayerCharacter::UnEquipWeapon()
{
	if (bIsEquip && !AnimInstance->Montage_IsPlaying(PlayerComboACK))
	{
		UWorld* world = GetWorld();
		DisableInput(UGameplayStatics::GetPlayerController(world, 0));
		if (AnimInstance && bIsEquip)
		{
			AnimInstance->Montage_Play(UnEquipAnim);
			MontageEndedDelagate.BindUObject(this, &APlayerCharacter::UnEquipEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate);
			bIsEquip = false;
			PlayerStateTemp->Set_NormalMode(PlayerCurrentMode);
			Set_NoneMode();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Not equip weapon!"));
		}
	}
}

UStaticMeshComponent* APlayerCharacter::GetWeapon()
{
	return this->Blade;
}

void APlayerCharacter::CheckPlayerAttack()//对玩家攻击产生射线检测。
{
	if (bIsAttacking)
	{
		//如果此时玩家在空中说明在进行空中连击
		if (CurrPlayerACKMode == EPlayerACKMode::EAirACKMode)
		{
			static float  dfs_plane = 0.f;
			dfs_plane = GetDisWithVector(FVector(GetActorLocation().X, GetActorLocation().Y, 0.f)) - 90.f;
			DebugNS::DurationDebug(0.5f, FString::SanitizeFloat(dfs_plane));
			if (dfs_plane <= 50.f && AnimInstance)
			{
				if (AnimInstance->Montage_IsPlaying(AirComboMontage))
				{
					GetWorldTimerManager().ClearTimer(Handler);
					CurrPlayerACKMode = ACKModeTemp;
					AnimInstance->Montage_Stop(0.1f, AirComboMontage);
				}
			}
		}

		UWorld* world = GetWorld();
		TargetTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
		TArray<AActor*> ActorToIngore;
		TArray<FHitResult> OutHits;
		for (int8 i = 0; i < SocketLocation.Num(); ++i)
		{
			if (UKismetSystemLibrary::LineTraceMultiForObjects(world, SocketLocation[i], GetWeapon()->GetSocketLocation(SocketNames[i]), TargetTypes, false, ActorToIngore, EDrawDebugTrace::None, OutHits, true))
			{
				AddTargetEnemy(OutHits);

				//如果击中并且在空中那么launch玩家，使玩家也抬高一段距离
				if(PlayerIsInAir())
					LaunchCharacter(FVector(0.f, 0.f, 400.f), false, true);
			}
		}
		InitSocketLocation();
	}
}

void APlayerCharacter::AddTargetEnemy(TArray<FHitResult> OutHits)
{
	if (OutHits.Num() != 0)
	{
		for (auto& result : OutHits)
		{
			ANormalEnemyCharacter* EnemyTemp = Cast<ANormalEnemyCharacter>(result.GetActor());
			if (EnemyTemp)
			{
				//对攻击到并添加进数组的敌人只执行一次黑板键的设置防止多次受击动画的产生
				if (!TargetEnemy.Contains(EnemyTemp))
				{
					//普通攻击打击音效
					USoundCue* HittedCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Weapons/wav/HitedCue.HitedCue"));
					if (HittedCue)
					{
						UGameplayStatics::PlaySound2D(GetWorld(), HittedCue, 2.0f);
						bIsPlayACKSound = true;
					}
					//相机晃动
					SetCameraShake();
					//打击感延迟
					ACKDelay();
					switch (HitAIType)
					{
					case EHitAIACKType::ComboHit:
						EnemyTemp->SetHittedType("ComboHit");
						break;
					case EHitAIACKType::ExcuteHit:
						EnemyTemp->SetHittedType("ExcuteHit");
						break;
					case EHitAIACKType::HitDown:
						EnemyTemp->SetHittedType("HitDown");
						break;
					case EHitAIACKType::HitFly:
					{
						if (EnemyTemp && EnemyTemp->GetCharacterMovement()->IsFalling())
							EnemyTemp->SetHittedType("HitDown");
						else
							EnemyTemp->SetHittedType("HitFly");
					}
						break;
					default:
						EnemyTemp->SetHittedType("Error!");
						break;
					}
					FVector HitLocation = result.ImpactPoint;
					PlayHitSpark(HitLocation);
					MakeDamage();
					EnemyTemp->SetHittedValue(true);
				}

				TargetEnemy.AddUnique(EnemyTemp);
			}
		}
	}
}

void APlayerCharacter::MontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "Combat1" || NotifyName == "Combat2" || NotifyName == "Combat3" || NotifyName == "Combat4")
	{
		GetWorldTimerManager().ClearTimer(Handler);
		GetWorldTimerManager().SetTimer(Handler, this, &APlayerCharacter::CheckPlayerAttack, GetWorld()->GetDeltaSeconds(), true);
	}
	if (NotifyName == "PlayerTrail1" || NotifyName == "PlayerTrail2" || NotifyName == "PlayerTrail3" || NotifyName == "PlayerTrail4")
	{
		GetWorldTimerManager().ClearTimer(TrailHandler);
		GetWorldTimerManager().SetTimer(TrailHandler, this, &APlayerCharacter::ShowTrail, GetWorld()->GetDeltaSeconds(), false);
	}
	if (NotifyName == "MidCombat")
	{
		GetWorldTimerManager().ClearTimer(MidAckHandler);
		GetWorldTimerManager().SetTimer(MidAckHandler, this, &APlayerCharacter::CheckPlayerAttack, GetWorld()->GetDeltaSeconds(), true);
	}
	if (NotifyName == "MidTrail")
	{
		GetWorldTimerManager().ClearTimer(TrailHandler);
		GetWorldTimerManager().SetTimer(TrailHandler, this, &APlayerCharacter::ShowTrail, GetWorld()->GetDeltaSeconds(), false);
	}
	if (NotifyName == "AirComb1" || NotifyName == "AirComb2" || NotifyName == "AirComb3" || NotifyName == "AirComb4")
	{
		InitSocketLocation();
		GetWorldTimerManager().ClearTimer(Handler);
		GetWorldTimerManager().SetTimer(Handler, this, &APlayerCharacter::CheckPlayerAttack, GetWorld()->GetDeltaSeconds(), true);
	}
	if (NotifyName == "ACKSkillNotify")
	{
		if (AnimInstance->Montage_IsPlaying(AirDownACK_Montage))
			bCheckDownACK = true;
		else
		{
			GetWorldTimerManager().ClearTimer(Handler);
			GetWorldTimerManager().SetTimer(Handler, this, &APlayerCharacter::CheckPlayerAttack, GetWorld()->GetDeltaSeconds(), true);
		}
	}
	if (NotifyName == "CanCombo")
	{
		bCanCombo = true;
	}
	if (NotifyName == "PlayerHittedBack")
	{
		GetWorldTimerManager().ClearTimer(FlashShadowHandler);
		if (AnimInstance->Montage_IsPlaying(CommonHittedMontage))
			HitBackTargetLocation = GetActorLocation() + (-GetActorForwardVector()) * 85.f;
		else if (AnimInstance->Montage_IsPlaying(HitFlyMontage))
			HitBackTargetLocation = GetActorLocation() + (-GetActorForwardVector()) * 500.f;
		GetWorldTimerManager().SetTimer(FlashShadowHandler, this, &APlayerCharacter::SetPlayerHittedBack, GetWorld()->GetDeltaSeconds(), true);
	}
}

void APlayerCharacter::MontageNotifyEnded(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "Combat1" || NotifyName == "Combat2" || NotifyName == "Combat3" || NotifyName == "Combat4")
	{
		TargetEnemy.Empty();
		GetWorldTimerManager().ClearTimer(Handler);
	}
	if (NotifyName == "PlayerTrail1" || NotifyName == "PlayerTrail2" || NotifyName == "PlayerTrail3" || NotifyName == "PlayerTrail4")
	{
		EndTrail();
		GetWorldTimerManager().ClearTimer(TrailHandler);
	}
	if (NotifyName == "MidCombat")
	{
		TargetEnemy.Empty();
		GetWorldTimerManager().ClearTimer(MidAckHandler);
	}
	if (NotifyName == "MidTrail")
	{
		EndTrail();
		GetWorldTimerManager().ClearTimer(TrailHandler);
	}
	if (NotifyName == "AirComb1" || NotifyName == "AirComb2" || NotifyName == "AirComb3" || NotifyName == "AirComb4")
	{
		TargetEnemy.Empty();
		GetWorldTimerManager().ClearTimer(Handler);
	}
	if (NotifyName == "ACKSkillNotify")
	{
		bCheckDownACK = false;
		GetWorldTimerManager().ClearTimer(Handler);
	}
	if (NotifyName == "CanCombo")
	{
		bCanCombo = false;
	}
	if (NotifyName == "PlayerHittedBack")
	{
		GetWorldTimerManager().ClearTimer(FlashShadowHandler);
	}
}

void APlayerCharacter::CheckMode(EPlayerStateMode& playerMode)
{
	switch (playerMode)
	{
	case EPlayerStateMode::ENORMAL:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("Normal"));
		break;
	case EPlayerStateMode::EEQUIP:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("Equip"));
		break;
	case EPlayerStateMode::EACK:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("Attack"));
		break;
	case EPlayerStateMode::EJUMP:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("Jump"));
		break;
	case EPlayerStateMode::ELOCK:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("Lock"));
		break;
	default:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("ErrorMode"));
	}
}

void APlayerCharacter::Show_ACKPose()
{
	switch (CurrPlayerACKMode)
	{
	case EPlayerACKMode::ENoneMode:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("NoneMode"));
		break;
	case EPlayerACKMode::EMidPoseMode:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("MidMode"));
		break;
	case EPlayerACKMode::EHighPoseMode:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("HighMode"));
		break;
	case EPlayerACKMode::EAirACKMode:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("AirACKMode"));
		break;
	case EPlayerACKMode::EDownACKMode:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("EDownACKMode"));
		break;
	case EPlayerACKMode::EUpperACKMode:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("EUpperACKMode"));
		break;
	default:
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("ErrorMode"));
		break;
	}
}

void APlayerCharacter::SetCheckCombo()
{
}

void APlayerCharacter::CombatStrikeSet(UAnimMontage* CurrentCombatMonateg)
{
	GetWorldTimerManager().SetTimer(StrikeHandler, this, &APlayerCharacter::StrikeTimeEnd, 0.3f, true);
	AnimInstance->Montage_SetPlayRate(CurrentCombatMonateg, 0.5);
}

void APlayerCharacter::StrikeTimeEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("INHere"));
	GetWorldTimerManager().ClearTimer(StrikeHandler);
	UAnimMontage* CurrMontage = AnimInstance->GetCurrentActiveMontage();
	AnimInstance->Montage_SetPlayRate(CurrMontage, 1.0f);
}


//极限闪避代码
void APlayerCharacter::SlideTo()
{
	if (!bEnergyEmpty)
	{
		UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
		if (AnimIns->Montage_IsPlaying(PlayerComboACK))
		{
			AnimIns->Montage_Stop(0.2f, PlayerComboACK);
		}
		if (AnimIns && PlayerLockMode == EPlayerStateMode::ELOCK)
		{
			if (!bIsSliding)
			{
				TakeEnergy(15.f);
				CurrentFlashType = EPlayerFlashType::ESlide;
				bIsSliding = true;
				bSlideTimming = true;
				//用闪避取消攻击的动作
				if (InputX == 0)
				{
					if (InputY == 0)
					{
						//后撤步
						AnimIns->Montage_Play(SlideMontages[ESldeType::ESlideB]);
						CurrentSldeType = ESldeType::ESlideB;
						AnimIns->OnMontageEnded.AddDynamic(this, &APlayerCharacter::SlideMontageEnd);
					}
					else if (InputY == 1.f)
					{
						AnimIns->Montage_Play(SlideMontages[ESldeType::ESlideF]);
						GetWorldTimerManager().SetTimer(FlashShadowHandler, this, &APlayerCharacter::StopSlideForward, GetWorld()->DeltaTimeSeconds, true);
						CurrentSldeType = ESldeType::ESlideF;
						AnimIns->OnMontageEnded.AddDynamic(this, &APlayerCharacter::SlideMontageEnd);
					}
					else if (InputY == -1.f)
					{
						AnimIns->Montage_Play(SlideMontages[ESldeType::ESlideB]);
						CurrentSldeType = ESldeType::ESlideB;
						AnimIns->OnMontageEnded.AddDynamic(this, &APlayerCharacter::SlideMontageEnd);
					}
				}
				else if (InputY == 0)
				{
					if (InputX == -1.f)
					{
						AnimIns->Montage_Play(SlideMontages[ESldeType::ESlideL]);
						CurrentSldeType = ESldeType::ESlideL;
						AnimIns->OnMontageEnded.AddDynamic(this, &APlayerCharacter::SlideMontageEnd);
					}
					else if (InputX == 1.f)
					{
						AnimIns->Montage_Play(SlideMontages[ESldeType::ESlideR]);
						CurrentSldeType = ESldeType::ESlideR;
						AnimIns->OnMontageEnded.AddDynamic(this, &APlayerCharacter::SlideMontageEnd);
					}
				}
				else if (InputY == 1.f)
				{
					if (InputX == -1.f)
					{
						AnimIns->Montage_Play(SlideMontages[ESldeType::ESlideLF]);
						CurrentSldeType = ESldeType::ESlideLF;
						AnimIns->OnMontageEnded.AddDynamic(this, &APlayerCharacter::SlideMontageEnd);
					}
					else if (InputX == 1.f)
					{
						AnimIns->Montage_Play(SlideMontages[ESldeType::ESlideRF]);
						CurrentSldeType = ESldeType::ESlideRF;
						AnimIns->OnMontageEnded.AddDynamic(this, &APlayerCharacter::SlideMontageEnd);
					}
				}
				else if (InputY == -1.f)
				{
					if (InputX == -1.f)
					{
						AnimIns->Montage_Play(SlideMontages[ESldeType::ESlideLB]);
						CurrentSldeType = ESldeType::ESlideLB;
						AnimIns->OnMontageEnded.AddDynamic(this, &APlayerCharacter::SlideMontageEnd);
					}
					else if (InputX == 1.f)
					{
						AnimIns->Montage_Play(SlideMontages[ESldeType::ESlideRB]);
						CurrentSldeType = ESldeType::ESlideRB;
						AnimIns->OnMontageEnded.AddDynamic(this, &APlayerCharacter::SlideMontageEnd);
					}
				}
				GetWorldTimerManager().SetTimer(SlideHandler, this, &APlayerCharacter::SlideTimeOut, 0.4f, false);
			}
		}
	}
}
void APlayerCharacter::SlideTimeOut()
{
	bSlideTimming = false;
	//在闪避完成后输入值重新设置为0
	CurrentX = 0;
	CurrentY = 0;
	GetWorldTimerManager().ClearTimer(SlideHandler);
}

void APlayerCharacter::CheckPerfectSlide()
{
	UWorld* world = GetWorld();

	if (bSlideTimming == true)
	{
		//完美闪避产生残影
		CurrentFlashType = EPlayerFlashType::EClosetFlash;
		SpawnShadowActor();
		bIsPerfectSliding = true;
		GetWorldTimerManager().SetTimer(TimeDilationHandler, this, &APlayerCharacter::TimeDilationOut, 1.f, false);
		//采用放慢蒙太奇的播放速度，这样不会影响到观感
		if (EnemyAIIns)
			EnemyAIIns->SlowEnemyAction();
	}
}
void APlayerCharacter::TimeDilationOut()
{
	bIsPerfectSliding = false;
	CurrentFlashType = EPlayerFlashType::ENULL;
	GetWorldTimerManager().ClearTimer(TimeDilationHandler);
}
float APlayerCharacter::GetInputX()
{
	return CurrentX;
}
float APlayerCharacter::GetInputY()
{
	return CurrentY;
}
void APlayerCharacter::JuHeSkill()
{
	if (!bEnergyEmpty)
	{
		TimeCounter += GetWorld()->DeltaTimeSeconds;
		if (TimeCounter <= 2.f && bLMouseButtonPressed == false)
		{
			GetWorldTimerManager().ClearTimer(TakeActionHandler);
			HitAIType = EHitAIACKType::HitDown;
			CurrentSkillType = ESkillType::ECYZ;
			AnimInstance->Montage_JumpToSection(FName("SkillOut"), JuHeMontage);
			bIsAttacking = true;
			TakeEnergy(10.f);
			SpawnJuHeEffect();
		}
		else if (TimeCounter > 2.f)
		{
			GetWorldTimerManager().ClearTimer(TakeActionHandler);
			HitAIType = EHitAIACKType::HitDown;
			bIsAttacking = true;
			TakeEnergy(30.f);
			float dis = GetDisWithActor(FinalLockTarget);
			CurrentSkillType = ESkillType::EJuHe;
			//如果居合时与AI距离已经很小了那么原地播放
			if (dis <= 200.f)
			{
				MontageEndedDelagate.BindUObject(this, &APlayerCharacter::AttackEnded);
				AnimInstance->Montage_JumpToSection(FName("SkillOut"), JuHeMontage);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate, JuHeMontage);
			}
			else if (dis > 200.f && dis <= 500.f)
			{
				JuHeDisType = ESkillType::EJuHe1;
				AnimInstance->Montage_JumpToSection(FName("SkillOut"), JuHeMontage);
				if (AnimInstance->Montage_GetCurrentSection(JuHeMontage) == FName("SkillOut"))
				{
					//普通居合产生位移
					GetWorldTimerManager().SetTimer(FlashShadowHandler, this, &APlayerCharacter::SetPlayerACK_MoveLocation, GetWorld()->DeltaTimeSeconds, true);
				}
			}
			else
			{
				JuHeDisType = ESkillType::EJuHe2;
				AnimInstance->Montage_JumpToSection(FName("SkillOut"), JuHeMontage);
				if (AnimInstance->Montage_GetCurrentSection(JuHeMontage) == FName("SkillOut"))
				{
					//普通居合产生位移
					GetWorldTimerManager().SetTimer(FlashShadowHandler, this, &APlayerCharacter::SetPlayerACK_MoveLocation, GetWorld()->DeltaTimeSeconds, true);
				}
			}
		}
	}
}
void APlayerCharacter::SpawnJuHeEffect()
{
	UWorld* world = GetWorld();
	FVector HitVector = FinalLockTarget->GetActorLocation();
	world->SpawnActor<ATestSkillActor>(ATestSkillActor::StaticClass(), HitVector, FRotator(0.0f));
}
AActor* APlayerCharacter::GetLockTarget()
{
	return FinalLockTarget;
}
void APlayerCharacter::ACKUpper()
{
		UWorld* world = GetWorld();
		AnimInstance->Montage_Play(UpperACKMontage);
		HitAIType = EHitAIACKType::ComboHit;
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::UpperMontageEnded);
}
bool APlayerCharacter::CheckUpperAI()
{
	FVector StartVector = GetActorLocation() + GetActorForwardVector() * 10.f;
	FVector EndVector = GetActorLocation() + GetActorForwardVector() * 80.f;
	FVector HalfSize = FVector(80.f, 50.f, 50.f);
	TArray<TEnumAsByte<	EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	TArray<AActor*> IngoreActor;
	TArray<FHitResult> HitResult;
	if (UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), StartVector, EndVector, HalfSize, GetActorRotation(), ObjectTypes, false, IngoreActor, EDrawDebugTrace::None, HitResult, true))
		if (HitResult.Num() != 0)
			for (const auto& Value : HitResult)
			{
				ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(Value.GetActor());
				if (AI)
				{
					//普通攻击打击音效
					USoundCue* HittedCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Weapons/wav/HitedCue.HitedCue"));
					if (HittedCue)
					{
						UGameplayStatics::PlaySound2D(GetWorld(), HittedCue, 2.0f);
						bIsPlayACKSound = true;
					}
					//相机晃动
					SetCameraShake();
					//打击感延迟
					ACKDelay();
					FVector HitLocation = Value.ImpactPoint;
					PlayHitSpark(HitLocation);
					AI->SetHittedValue(true);	//使AI受击
					if (AI->GetBodyState() == EBossStateType::QJ)
					{
						UpperACKTargetArr.AddUnique(AI);
						return true;
					}
					else
						return false;
				}
			}
	return false;
}
void APlayerCharacter::UpperPlayer()
{
	LaunchCharacter(FVector(0, 0, 700.f), false, false);
	/*DebugNS::MyDebug(3.0f, FString("UpperPlayer"));
	FVector FinalVector(GetActorLocation().X, GetActorLocation().Y, 500.f);
	FVector SetVector = UKismetMathLibrary::VInterpTo(GetActorLocation(), FinalVector, GetWorld()->GetDeltaSeconds(), 10);
	if (GetActorLocation().Z >= 500)
		GetWorldTimerManager().ClearTimer(UpperPlayerHandler);*/

}

void APlayerCharacter::SetDownACK()
{
	if (bDownACK)
		bDownACK = false;
	else
		bDownACK = true;
}

void APlayerCharacter::UpperMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance->Montage_IsPlaying(AirDownACK_Montage) || AnimInstance->Montage_IsPlaying(AirComboMontage) ||
		AnimInstance->Montage_IsPlaying(JuHeMontage))
	{
		PlayerCurrentMode = EPlayerStateMode::EACK;
		if (AnimInstance->Montage_IsPlaying(AirComboMontage))
			CurrPlayerACKMode = EPlayerACKMode::EAirACKMode;
		if (AnimInstance->Montage_IsPlaying(AirDownACK_Montage))
			CurrPlayerACKMode = EPlayerACKMode::EDownACKMode;
	}
	else
		PlayerCurrentMode = EPlayerStateMode::EEQUIP;
	if(CurrPlayerACKMode != EPlayerACKMode::EAirACKMode && CurrPlayerACKMode != EPlayerACKMode::EDownACKMode)
		CurrPlayerACKMode = ACKModeTemp;
	UpperACKTargetArr.Empty();
}
TArray<AActor*> APlayerCharacter::GetUpperTarget()
{
	return UpperACKTargetArr;
}
bool APlayerCharacter::PlayerIsInAir()
{
	if (GetCharacterMovement()->IsFalling())
	{
		//DebugNS::DurationDebug(3.0f, FString("InAir!"));
		return true;
	}
	else
	{
		//DebugNS::DurationDebug(3.0f, FString("NotInAir!"));
		return false;
	}
}

void APlayerCharacter::PlayNextMontage(const TArray<UAnimMontage*>& MontageArray, UAnimMontage* CurrentAnimMontage)
{
	for (int i = 0; i < MontageArray.Num(); ++i)
	{
		if (MontageArray[i] == CurrentAnimMontage && bCanCombo)
		{
			if(i == MontageArray.Num())
				CurrentAnimMontage = MontageArray[0];
			else
				CurrentAnimMontage = MontageArray[i + 1];
			MontageEndedDelagate.BindUObject(this, &APlayerCharacter::AttackEnded);
			AnimInstance->Montage_Play(CurrentAnimMontage);
			HitAIType = EHitAIACKType::ComboHit;
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate);
			bIsAttacking = true;
			TakeEnergy(15.f);
		}
	}
}

void APlayerCharacter::FlashPlayer()
{
	//只有在锁定的时候才可以瞬身
	if (PlayerLockMode == EPlayerStateMode::ELOCK && !bEnergyEmpty)
	{
		CurrentFlashType = EPlayerFlashType::EFlashBody;
		//锁定前跳前瞬或者后翻滚
		if (InputY == 1.f)
		{
			if (AnimInstance && FlashBodyMontage)
				AnimInstance->Montage_Play(FlashBodyMontage);
		}
		else if(InputY == -1.f && !PlayerIsInAir())
		{
			if (AnimInstance && RollBackMontage)
			{
				AnimInstance->Montage_Play(RollBackMontage);
				bIsBackroll = true;
				TakeEnergy(20.f);
				AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::RollBackEnded);
			}
		}
	}
}



void APlayerCharacter::SetPlayerMoveLoc()
{
	GetWorldTimerManager().ClearTimer(FlashShadowHandler);
	GetWorldTimerManager().SetTimer(FlashShadowHandler, this, &APlayerCharacter::SetPlayerFlashLocation, GetWorld()->GetDeltaSeconds(), true);
}

void APlayerCharacter::SetPlayerFlashLocation()
{
	FVector FinalLocation = GetActorLocation() + GetActorForwardVector() * 10.f;
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), FinalLocation, GetWorld()->GetDeltaSeconds(), 100.f));
	SpawnShadowActor();
	//判断如果玩家和AI的距离过小那么应该停止瞬移
	if (FinalLockTarget && GetDisWithActor(FinalLockTarget) < 150.f)
	{
		AnimInstance->Montage_Stop(0.1f, FlashBodyMontage);
		ClearFlashLocationEndHandler();
	}
}

void APlayerCharacter::ClearFlashLocationEndHandler()
{
	springArm->CameraLagSpeed = 3.f;
	CurrentFlashType = EPlayerFlashType::ENULL;
	GetWorldTimerManager().ClearTimer(FlashShadowHandler);
}



void APlayerCharacter::SetCameraDelay()
{
	//如果浮空那么不需要摄像机延迟
	if(PlayerIsInAir() || bHitted || bIsSliding || bIsBackroll)
		springArm->CameraLagSpeed = 0;
	else
		springArm->CameraLagSpeed = 3;
}

void APlayerCharacter::SpawnShadowActor()
{
	FVector NewFvector = { GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z - 90.f };
	FRotator NewRotator = { GetActorRotation().Pitch,GetActorRotation().Yaw - 90.f,GetActorRotation().Roll };
	GetWorld()->SpawnActor<AFlashBodyEffect>(AFlashBodyEffect::StaticClass(), NewFvector, NewRotator);
}

EPlayerFlashType APlayerCharacter::GetPlayerFlashType()
{
	return CurrentFlashType;
}

void APlayerCharacter::SpawnFollowRobot()
{
	FVector	SpawnVector = { GetActorLocation() - GetActorForwardVector() * 30.f - GetActorRightVector() * 70.f + GetActorUpVector() * 70.f };
	//SpawnVector.Z += 70.f;
	Robot = GetWorld()->SpawnActor<AAutoRobot>(AAutoRobot::StaticClass(), SpawnVector, GetActorRotation());
}

bool APlayerCharacter::IsMoveOrJump()
{
	if(InputX != 0 || InputY != 0 || PlayerIsInAir() || bIsOpenFire)
		return true;
	return false;
}

void APlayerCharacter::RollBackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance->Montage_IsPlaying(FlashBodyMontage))
		CurrentFlashType = EPlayerFlashType::EFlashBody;
	else if (Montage == RollBackMontage)
		CurrentFlashType = EPlayerFlashType::ENULL;
	bIsBackroll = false;
}

void APlayerCharacter::RobotOpenFire()
{
	if (!bEnergyEmpty)
	{
		GetWorldTimerManager().SetTimer(AutoFireHandler, this, &APlayerCharacter::BackCallOpenFire, 0.1f, true);
		bIsOpenFire = true;
		bIsAttacking = true;
	}
}

void APlayerCharacter::BackCallOpenFire()
{
	TakeEnergy(3.f);
	if (bEnergyEmpty)
	{
		GetWorldTimerManager().ClearTimer(AutoFireHandler);
		bIsAttacking = false;
		bIsOpenFire = false;
		return;
	}
	if (Robot)
		Robot->OpenFire();
}

void APlayerCharacter::RobotStopFire()
{
	GetWorldTimerManager().ClearTimer(AutoFireHandler);
	bIsAttacking = false;
	bIsOpenFire = false;
}

bool APlayerCharacter::IsRobotOpenFire()
{
	return bIsOpenFire;
}

bool APlayerCharacter::IsLock()
{
	if (PlayerLockMode == EPlayerStateMode::ELOCK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void APlayerCharacter::UnlockWhenTooFar()
{
	if (FinalLockTarget)
	{
		float dis = std::abs((FinalLockTarget->GetActorLocation() - GetActorLocation()).Size());
		//当锁定目标大于二十米时取消锁定
		ANormalEnemyCharacter* Enemy = Cast<ANormalEnemyCharacter>(FinalLockTarget);
		if (dis > 2000.f)
			Unlock(Enemy);
	}
}

void APlayerCharacter::DownACKChecker()
{
	static float dis = 0.f;
	dis = GetDisWithVector(FVector(GetActorLocation().X, GetActorLocation().Y, 0.f)) - 90.f;
	if (dis <= 50.f)
	{
		if (AnimInstance->Montage_GetCurrentSection() == FName("ACKStart") || AnimInstance->Montage_GetCurrentSection() == FName("ACKLoop"))
			AnimInstance->Montage_JumpToSection(FName("ACKEnd"), AirDownACK_Montage);
	}

}

void APlayerCharacter::Unlock(ANormalEnemyCharacter* EnemyTemp)
{
	GetWorldTimerManager().ClearTimer(LockHandler);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//UE_LOG(LogTemp, Warning, TEXT("ClearHandler"));
	EnemyTemp->SetLockMeshVisible();
	FinalLockTarget = nullptr;
	bIsLock = false;
	if (bIsEquip)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		PlayerStateTemp->Set_EquipMode(PlayerLockMode);
	}
}

AAutoRobot* APlayerCharacter::GetRobot()
{
	return Robot;
}

void APlayerCharacter::SetPlayerUIRotation()
{
	if (PlayerUiWidget)
	{
		float Yaw = GetControlRotation().Yaw + 180.f;
		PlayerUiWidget->SetWorldRotation(FRotator(GetActorRotation().Pitch, Yaw, GetActorRotation().Roll));
	}
}

float APlayerCharacter::GetHP()
{
	return HP;
}

float APlayerCharacter::GetSP()
{
	return SP;
}

bool APlayerCharacter::GetPressF()
{
	return bPressF;
}

void APlayerCharacter::TakeAction()
{
	bPressF = true;
	GetWorldTimerManager().SetTimer(TakeActionHandler,this, &APlayerCharacter::SetTakeAction, 0.5f, false);
}

void APlayerCharacter::SetTakeAction()
{
	GetWorldTimerManager().ClearTimer(TakeActionHandler);
	if (bPressF)
		bPressF = false;
}

void APlayerCharacter::OverShoulderView()
{
	camera->SetRelativeLocation(FVector(220.f,30.f,0.f));
}

void APlayerCharacter::AirFlash()
{
	if (FinalLockTarget)
	{
		FVector TargetLocation = FinalLockTarget->GetActorLocation() + FinalLockTarget->GetActorForwardVector() * 100.f;
		SetActorLocation(FVector(TargetLocation.X, TargetLocation.Y, TargetLocation.Z + 200.f));
		GetWorldTimerManager().ClearTimer(FlashShadowHandler);
		GetWorldTimerManager().SetTimer(FlashShadowHandler, this, &APlayerCharacter::AirFlashSapwnShadow,GetWorld()->DeltaTimeSeconds, true);
	}
}

void APlayerCharacter::AirFlashSapwnShadow()
{
	if (!PlayerIsInAir())
	{
		GetWorldTimerManager().ClearTimer(FlashShadowHandler);
		return;
	}
	else if (PlayerIsInAir() && AnimInstance->Montage_IsPlaying(AirComboMontage))
	{
		GetWorldTimerManager().ClearTimer(FlashShadowHandler);
		return;
	}
	else if (PlayerIsInAir() && AnimInstance->Montage_IsPlaying(AirDownACK_Montage))
	{
		GetWorldTimerManager().ClearTimer(FlashShadowHandler);
		return;
	}
	SpawnShadowActor();
}

void APlayerCharacter::ResetCameraLocation()
{
	camera->SetRelativeLocation(FVector(0,0,0));
}

void APlayerCharacter::PlaySitUp()
{
	if (PlayerSitUp)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(PlayerSitUp);
		GetWorldTimerManager().SetTimer(MontageDelayHandler, this, &APlayerCharacter::SetNormalInputMode, 1.5f, false);
	}
}

void APlayerCharacter::SetNormalInputMode()
{
	//在角色完成起身动画后回到正常输入模式,以及设置回玩家相机视角
	 GetMesh()->GetAnimInstance()->Montage_Stop(0.f, PlayerSitUp);
	 AMainPlayerController* pc = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	 if (pc)
	 {
		 pc->SetNormalInputMode();
		 pc->SetViewTargetWithBlend(this,1.f);
	 }
	 GetWorldTimerManager().ClearTimer(MontageDelayHandler);
}

void APlayerCharacter::SwitchMagicItem()
{
	AMainPlayerController* pc = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pc)
		pc->SwitchMagicItem();
}

void APlayerCharacter::SwitchMedcineItem()
{
	AMainPlayerController* pc = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pc)
		pc->SwitchMedcineItem();
}

TMap<FString, int>& APlayerCharacter::GetItemNum()
{
	return ItemNum;
}

float APlayerCharacter::GetDisWithActor(AActor* Target)
{
	float result = std::abs((GetActorLocation() - Target->GetActorLocation()).Size());
	return result;
}

float APlayerCharacter::GetDisWithVector(FVector TargetLocation)
{
	float result = std::abs((GetActorLocation() - TargetLocation).Size());
	return result;
}

void APlayerCharacter::DisplayPlayerMode()
{
	switch (PlayerCurrentMode)
	{
	case EPlayerStateMode::ENORMAL:
		DebugNS::DurationDebug(1.f, FString("PlayerState :") + FString("ENORMAL"));
		break;
	case EPlayerStateMode::EEQUIP:
		DebugNS::DurationDebug(1.f, FString("PlayerState :") + FString("EEQUIP"));
		break;
	case EPlayerStateMode::EACK:
		DebugNS::DurationDebug(1.f, FString("PlayerState :") + FString("EACK"));
		break;
	case EPlayerStateMode::ELOCK:
		DebugNS::DurationDebug(1.f, FString("PlayerState :") + FString("ELOCK"));
		break;
	case EPlayerStateMode::EJUMP:
		DebugNS::DurationDebug(1.f, FString("PlayerState :") + FString("EJUMP"));
		break;
	default:
		break;
	}
}

void APlayerCharacter::PlayHittedMontage(EPlayerHittedType Type)
{
	bHitted = true;
	//先把玩家转向AI
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyAIIns->GetActorLocation()));
	MontageEndedDelagate.BindUObject(this, &APlayerCharacter::HittedMontageEnd);
	switch (Type)
	{
	case EPlayerHittedType::CommonHit:
		AnimInstance->Montage_Play(CommonHittedMontage);
		break;
	case EPlayerHittedType::HitFly:
		AnimInstance->Montage_Play(HitFlyMontage);
		break;
	default:
		DebugNS::MyDebug(1.f, "ErrorHittedMontage");
		break;
	}
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelagate);
}

void APlayerCharacter::HittedMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	GetWorldTimerManager().ClearTimer(FlashShadowHandler);
	//如果if通过说明受击动画被新的受击打断，设置布尔值仍为true
	if (AnimInstance->Montage_IsPlaying(CommonHittedMontage) || AnimInstance->Montage_IsPlaying(HitFlyMontage))
		bHitted = true;
	else
		bHitted = false;
}

void APlayerCharacter::PushPlayerToAI()
{
	if (GetDisWithActor(EnemyAIIns) <= 150.f)
	{
		GetWorldTimerManager().ClearTimer(StrikeHandler);
		return;
	}
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), EnemyAIIns->GetActorLocation(), GetWorld()->DeltaTimeSeconds, 2.f));
}

void APlayerCharacter::SetSafeDisWithAI()
{
	if (!PlayerIsInAir() && bIsAttacking && GetDisWithActor(EnemyAIIns) <= 100.f)
	{
		FVector location = GetActorLocation() - EnemyAIIns->GetActorLocation();
		if (location.Normalize())
			SetActorLocation(EnemyAIIns->GetActorLocation() + location * 100.f);
	}
}

bool APlayerCharacter::SkillReadyProgress()
{
	if (bIsLock && bLShiftIsPressed && InputY == 1.f)
		return true;
	else if (bIsLock && bLShiftIsPressed && InputY == -1.f)
		return true;
	else
		return false;
}

void APlayerCharacter::UseSkillPoints()
{
	if (SkillPoints == 0)
	{
		DebugNS::MyDebug(1.f, "No SkillPoint!");
		return;
	}
	else
	{
		SkillPoints--;
	}
}

void APlayerCharacter::HealSkillPoints()
{
	if (SkillPoints < 5 && !bHealSkillPointInProgress)
	{
		bHealSkillPointInProgress = true;
		GetWorldTimerManager().SetTimer(SkillPointsHealHandler, this, &APlayerCharacter::HealSkillPointsDelegate, 3.f, false);
	}
}

void APlayerCharacter::HealSkillPointsDelegate()
{
	GetWorldTimerManager().ClearTimer(SkillPointsHealHandler);
	SkillPoints++;
	bHealSkillPointInProgress = false;
}

void APlayerCharacter::TakeHealth(float DamageValue)
{
	if (!PlayerDied)
	{
		HP = FMath::Clamp<float>(HP - DamageValue, 0.f, 100.f);
		if (HP - 0.f <= 0.001f)
		{
			PlayerDied = true;
			if (PCIns)
			{
				AMainHUD* HudIns = Cast<AMainHUD>(PCIns->GetHUD());
				if (HudIns)
				{
					PCIns->SetUIInputMode();
					HudIns->AddGameOverWidgetToViewPort();
				}
			}
		}
	}
}

void APlayerCharacter::TakeEnergy(float CostValue)
{
	//先判断是否已经为空
	if (SP-0.f >= 0.001f)
	{
		if (SP > 0)
			SP -= CostValue;
		if (SP <= 0)
		{
			bEnergyEmpty = true;
			SP = 0;
			//如果体力空后两秒再进行恢复
			GetWorldTimerManager().SetTimer(EnergyEmptyTimer, this, &APlayerCharacter::ClearHealHandler, 2.f, false);
		}
	}
}

void APlayerCharacter::HealEnergy()
{
	if (!bEnergyEmpty && !bIsAttacking)
	{
		if (SP < 100.f && (GetVelocity().Size() - 0.f) < 0.001f)
		{
			SP += 0.3;
		}
		else if (SP < 100.f && GetVelocity().Size() >= 100.f)
		{
			SP += 0.2;
		}
		if (SP > 100)
			SP = 100;
	}
}

void APlayerCharacter::ClearHealHandler()
{
	bEnergyEmpty = false;
	GetWorldTimerManager().ClearTimer(EnergyEmptyTimer);
}

void APlayerCharacter::DownACKCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && bCheckDownACK)
	{
		ANormalEnemyCharacter* AI = Cast<ANormalEnemyCharacter>(OtherActor);
		if (AI)
		{
			bCheckDownACK = false;
			if (AI->GetCharacterMovement()->IsFalling())
				AI->SetHittedType("HitDown");
			else
				AI->SetHittedType("HitFly");
			//普通攻击打击音效
			USoundCue* HittedCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Weapons/wav/HitedCue.HitedCue"));
			if (HittedCue)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), HittedCue, 2.0f);
				bIsPlayACKSound = true;
			}
			//相机晃动
			SetCameraShake();
			//打击感延迟
			ACKDelay();
			FVector HitLocation = SweepResult.ImpactPoint;
			PlayHitSpark(HitLocation);
			AI->SetHittedValue(true);
		}
	}
}

void APlayerCharacter::StopSlideForward()
{
	if (GetDisWithActor(EnemyAIIns) - 100.f < 0.1f)
	{
		AnimInstance->Montage_Stop(0.f, SlideMontages[ESldeType::ESlideF]);
	}
}

void APlayerCharacter::ACKDelay()
{
	if (CurrentACKMontage)
	{
		AnimInstance->Montage_SetPlayRate(CurrentACKMontage, 0.3f);
		GetWorldTimerManager().ClearTimer(StrikeHandler);
		GetWorldTimerManager().SetTimer(StrikeHandler, this, &APlayerCharacter::ACKDelayOver, 0.1f, false);
	}
}

void APlayerCharacter::ACKDelayOver()
{
	GetWorldTimerManager().ClearTimer(StrikeHandler);
	AnimInstance->Montage_SetPlayRate(CurrentACKMontage, 1.f);
}

bool APlayerCharacter::GetPlayACKSoundBoolean()
{
	return bIsPlayACKSound;
}

void APlayerCharacter::PlaySwingSound()
{
	static USoundCue* sound = nullptr;
	if (CurrentACKMontage == PlayerComboACK)
		sound = LoadObject<USoundCue>(this, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Swings_and_Whoosh/wav/HighACKCue.HighACKCue"));
	else if(CurrentACKMontage == MidAckMontage)
		sound = LoadObject<USoundCue>(this, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Swings_and_Whoosh/wav/MidACKCue.MidACKCue"));
	if (sound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), sound);
	}
}

void APlayerCharacter::PlayHitSpark(FVector HitLocation)
{
	UParticleSystem* SparkSystem = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/StarterContent/Particles/P_Sparks.P_Sparks"));
	if (SparkSystem)
	{
		FTransform NewTrans = { FRotator(),HitLocation,FVector(1.f,1.f,1.f) };
		UParticleSystemComponent* system1 = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, NewTrans, false);
		//带参数的定时器绑定写法
		FTimerDelegate SparkTimerDelegate;
		SparkTimerDelegate.BindUObject(this, &APlayerCharacter::HitSparkOver, system1);
		GetWorldTimerManager().ClearTimer(SpawnHitSparkHandler);
		GetWorldTimerManager().SetTimer(SpawnHitSparkHandler,SparkTimerDelegate,0.1f,false);
	}
}

void APlayerCharacter::HitSparkOver(UParticleSystemComponent* system)
{
	GetWorldTimerManager().ClearTimer(SpawnHitSparkHandler);
	if(system->IsActive())
		system->Deactivate();
}

void APlayerCharacter::FireHealPlayerItem()
{
	ItemNum["FirePaper"] = 2;
	ItemNum["LightPaper"] = 2;
	ItemNum["HP_Medcine"] = 2;
	ItemNum["SP_Medcine"] = 2;

}

void APlayerCharacter::UseItem()
{
	FString CurrentItem = PCIns->GetCurrentItem();
	//锁定状态下才可以使用Debuff道具
	if (PCIns && FinalLockTarget)
	{
		if (CurrentItem == "FirePaper" || CurrentItem == "LightPaper")
		{
			if (CurrentItem == "FirePaper" && ItemNum["FirePaper"] != 0 && EnemyAIIns)
			{
				USoundCue* FirePaperCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Magic/wav/FirePaper.FirePaper"));
				if (FirePaperCue)
					UGameplayStatics::PlaySound2D(GetWorld(), FirePaperCue);
				ItemNum["FirePaper"]--;
				PCIns->SetItemNum(ItemNum["FirePaper"]);
				EnemyAIIns->SetBurnEffect();
			}
			else if (CurrentItem == "LightPaper" && ItemNum["LightPaper"] != 0 && EnemyAIIns)
			{
				USoundCue* LightPaperCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Magic/wav/LightPaper.LightPaper"));
				if (LightPaperCue)
					UGameplayStatics::PlaySound2D(GetWorld(), LightPaperCue);
				ItemNum["LightPaper"]--;
				PCIns->SetItemNum(ItemNum["LightPaper"]);
				EnemyAIIns->SetLightingEffect();
			}
			else
				DebugNS::MyDebug(1.f, "No Debuff Item");
		}
	}
	//普通恢复道具
	if(PCIns)
	{
		if (CurrentItem == "HP_Medcine" || CurrentItem == "SP_Medcine")
		{
			if (CurrentItem == "HP_Medcine" && ItemNum["HP_Medcine"] != 0)
			{
				USoundCue* MedcineHeal = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Magic/wav/MedcineHeal.MedcineHeal"));
				if (MedcineHeal)
					UGameplayStatics::PlaySound2D(GetWorld(), MedcineHeal);
				ItemNum["HP_Medcine"]--;
				PCIns->SetItemNum(ItemNum["HP_Medcine"]);
				HP = FMath::Clamp<float>(HP + 50.f, 0.f, 100.f);
			}
			else if (CurrentItem == "SP_Medcine" && ItemNum["SP_Medcine"] != 0)
			{
				USoundCue* MedcineHeal = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Magic/wav/MedcineHeal.MedcineHeal"));
				if (MedcineHeal)
					UGameplayStatics::PlaySound2D(GetWorld(), MedcineHeal);
				ItemNum["SP_Medcine"]--;
				PCIns->SetItemNum(ItemNum["SP_Medcine"]);
				SP = FMath::Clamp<float>(SP + 50.f, 0.f, 100.f);
			}
			else
				DebugNS::MyDebug(1.f, "No Medcine Item");
		}
	}
}

void APlayerCharacter::MakeDamage()
{
	if (CurrentMakeDamgeMontage != nullptr && EnemyAIIns && PlayerData)
	{
		if (CurrentMakeDamgeMontage == PlayerComboACK)
		{
			EnemyAIIns->TakeDamageAndAmmor(PlayerData->HighACKDmg);
			return;
		}
		else if (CurrentMakeDamgeMontage == MidAckMontage)
		{
			EnemyAIIns->TakeDamageAndAmmor(PlayerData->MidACKDmg);
			return;
		}
		else if (CurrentMakeDamgeMontage == AirComboMontage)
		{
			EnemyAIIns->TakeDamageAndAmmor(PlayerData->AirACKDmg);
			return;
		}
		else if (CurrentMakeDamgeMontage == JuHeMontage)
		{
			if (CurrentSkillType == ESkillType::EJuHe)
			{
				EnemyAIIns->TakeDamageAndAmmor(PlayerData->JuheDmg);
				return;
			}
		}
	}
}


void APlayerCharacter::SetPlayerHittedBack()
{
	if (AnimInstance->Montage_IsPlaying(CommonHittedMontage))
		SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), HitBackTargetLocation, GetWorld()->DeltaTimeSeconds, 10.f));
	else if (AnimInstance->Montage_IsPlaying(HitFlyMontage))
		SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), HitBackTargetLocation, GetWorld()->DeltaTimeSeconds, 5.f));
}

void APlayerCharacter::SlideMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	GetWorldTimerManager().ClearTimer(FlashShadowHandler);
	if (AnimInstance->Montage_IsPlaying(FlashBodyMontage))
		CurrentFlashType = EPlayerFlashType::EFlashBody;
	else
		CurrentFlashType = EPlayerFlashType::ENULL;
	CurrentSldeType = ESldeType::ENULL;
	bIsSliding = false;
}

void APlayerCharacter::KeysNeedToToggleOrSet(TArray<FKey>& Keys, bool bIngore)
{
	if (PCIns)
		for (auto key : Keys)
			PCIns->ToggleInputKey(bIngore, key);
}

void APlayerCharacter::SetPlayerACK_MoveLocation()
{
	static FVector TargetLocation = FVector();
	TargetLocation = GetActorLocation() + GetActorForwardVector() * JuHeDis[JuHeDisType];
	static float dis = 0.f;
	dis = GetDisWithVector(TargetLocation);
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, GetWorld()->DeltaTimeSeconds,50.f));
	if (GetActorLocation().Equals(TargetLocation))
	{
		GetWorldTimerManager().ClearTimer(FlashShadowHandler);
		JuHeDisType = ESkillType::ENULL;
		CurrentSkillType = ESkillType::ENULL;
		PlayerCurrentMode = EPlayerStateMode::EEQUIP;
		return;
	}
	else if (GetDisWithActor(FinalLockTarget) <= 150.f)
	{
		GetWorldTimerManager().ClearTimer(FlashShadowHandler);
		JuHeDisType = ESkillType::ENULL;
		CurrentSkillType = ESkillType::ENULL;
		PlayerCurrentMode = EPlayerStateMode::EEQUIP;
		//GetMesh()->GetAnimInstance()->Montage_Stop(0.f,JuHeMontage);
		return;
	}
}

void APlayerCharacter::InitlizeItemNums()
{
	ItemNum.Add("FirePaper", 0);
	ItemNum.Add("LightPaper", 0);
	ItemNum.Add("HP_Medcine", 0);
	ItemNum.Add("SP_Medcine", 0);
}


bool APlayerCharacter::GetDownACKBool()
{
	return bDownACK;
}
void APlayerCharacter::DownACKTimer()
{
	if (!PlayerIsInAir())	//此时玩家已落地,那么把落地攻击的判断条件设置为false。
	{
		PlayerCurrentMode = EPlayerStateMode::EEQUIP;
		SetDownACK();
		GetWorldTimerManager().ClearTimer(DownACKHandler);
	}
}
/**/


void APlayerCharacter::LockSystem()
{
	if (PlayerCurrentMode == EPlayerStateMode::EEQUIP && PlayerLockMode == EPlayerStateMode::EEQUIP || PlayerCurrentMode == EPlayerStateMode::EACK || PlayerLockMode == EPlayerStateMode::ELOCK)
	{
		TargetActorArray.Empty();
		DifZValueArr.Empty();
		FindTargetEnemy();
		if (TargetActorArray.Num() != 0)
		{
			for (auto& value : TargetActorArray)
			{
				DifZValueArr.AddUnique(value.Value);//将map的每个value值加入数组，以保持数组元素排列与Map一致。
			}
			float MinValue = DifZValueArr[0];
			for (int i = 0; i < DifZValueArr.Num(); ++i)
			{
				if (DifZValueArr[i] < MinValue)
					MinValue = DifZValueArr[i];
			}
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString(FString::SanitizeFloat(MinValue)));
			for (auto& value : TargetActorArray)
			{
				if (value.Value == MinValue)
				{
					FinalLockTarget = value.Key;
					break;
				}
			}
			LockLastCheck();
			if (FinalLockTarget)//最终锁定目标存在
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString(FinalLockTarget->GetName()));
				ANormalEnemyCharacter* EnemyTemp = Cast<ANormalEnemyCharacter>(FinalLockTarget);
				if (EnemyTemp)//转换成功
				{
					UStaticMeshComponent* LockMesh = EnemyTemp->GetLockMesh();
					if (LockMesh->IsVisible())//如果已经锁定
					{
						Unlock(EnemyTemp);
						return;
					}
					//没有锁定
					PlayerStateTemp->Set_LockMode(PlayerLockMode);//玩家当前状态设置为锁定状态
					bIsLock = true;
					GetCharacterMovement()->MaxWalkSpeed = 400.0f;
					EnemyTemp->SetLockMeshVisible();
					GetCharacterMovement()->bOrientRotationToMovement = false;
					GetWorldTimerManager().SetTimer(LockHandler, this, &APlayerCharacter::MakeLockRotation, GetWorld()->GetDeltaSeconds(), true);
				}
			}
		}
	}
}

void APlayerCharacter::LockLastCheck()//对路径上做球形检测以判断有无其它遮挡敌人
{
	if (FinalLockTarget)
	{
		UWorld* world = GetWorld();
		FVector CameraLocation = camera->GetComponentLocation();
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
		ObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
		TArray<AActor*> IngoreArr;
		IngoreArr.AddUnique(FinalLockTarget);//忽略最初的锁定目标。
		TArray<FHitResult> HitResult;
		if (UKismetSystemLibrary::SphereTraceMultiForObjects(world, CameraLocation, FinalLockTarget->GetActorLocation(), 50.0f, ObjectTypes, false, IngoreArr, EDrawDebugTrace::None, HitResult, true))
		{
			if (HitResult.Num() != 0)
				for (auto& Value : HitResult)
				{
					FinalLockTarget = Value.GetActor();//获取最终的锁定目标
					break;
				}
		}
	}
}

void APlayerCharacter::MakeLockRotation()
{
	FRotator DifRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FinalLockTarget->GetActorLocation());
	FRotator NewRotator = { GetActorRotation().Pitch, DifRotator.Yaw, GetActorRotation().Roll };
	FRotator PlayerRotator = UKismetMathLibrary::RInterpTo(GetActorRotation(), NewRotator,GetWorld()->GetDeltaSeconds(),15.0f);

	FRotator DIfCamRotator = UKismetMathLibrary::FindLookAtRotation(camera->GetComponentLocation(), FinalLockTarget->GetActorLocation());
	FRotator NewCamRotator = { GetControlRotation().Pitch,DIfCamRotator .Yaw,GetControlRotation().Roll};
	//相机转动频率高且幅度微小时必须将插值速度设置小
	FRotator CameraRotator = UKismetMathLibrary::RInterpTo(GetControlRotation(), NewCamRotator, GetWorld()->GetDeltaSeconds(), 1.0f);
	GetController()->SetControlRotation(NewCamRotator);
	SetActorRotation(PlayerRotator);
}

TArray<ANormalEnemyCharacter*> APlayerCharacter::GetTargetEnemy()
{
	return TargetEnemy;
}

void APlayerCharacter::ShowTrail()
{
	ParticleSystemComp->BeginTrails("Socket_Begin", "Socket_End", ETrailWidthMode_FromFirst, 1.0f);
	ParticleSystemComp->SetFloatParameter("LifeTime",0.5f);
}

void APlayerCharacter::EndTrail()
{
	ParticleSystemComp->EndTrails();
}

void APlayerCharacter::Set_NoneMode()
{
	CurrPlayerACKMode = EPlayerACKMode::ENoneMode;
}

void APlayerCharacter::Set_MidACKMode()
{
	if (PCIns)
	{
		FString mode("Mid");
		PCIns->SwitchACKMode(mode);
	}
	CurrPlayerACKMode = EPlayerACKMode::EMidPoseMode;
}

void APlayerCharacter::Set_HighACKMode()
{
	if (PCIns)
	{
		FString mode("High");
		PCIns->SwitchACKMode(mode);
	}
	CurrPlayerACKMode = EPlayerACKMode::EHighPoseMode;
}

void APlayerCharacter::Switch_ACKPose()
{
	if (CurrPlayerACKMode != EPlayerACKMode::ENoneMode)
	{
		if (CurrPlayerACKMode == EPlayerACKMode::EMidPoseMode)
		{
			ACKModeTemp = EPlayerACKMode::EHighPoseMode;
			Set_HighACKMode();
		}
		else if (CurrPlayerACKMode == EPlayerACKMode::EHighPoseMode)
		{
			ACKModeTemp = EPlayerACKMode::EMidPoseMode;
			Set_MidACKMode();
		}
	}
}

void APlayerCharacter::FindTargetEnemy()
{
	UWorld* world = GetWorld();
	TArray<AActor*> ActorOfClass;
	UGameplayStatics::GetAllActorsOfClass(world, ANormalEnemyCharacter::StaticClass(), ActorOfClass);
	for (AActor* ptr : ActorOfClass)
	{
		float Dis = GetDisWithActor(ptr);
		if (Dis <= 1500)
		{
			FRotator CameraToPlayer = UKismetMathLibrary::FindLookAtRotation(camera->GetComponentLocation(), GetActorLocation());
			FRotator CameraToEnemy = UKismetMathLibrary::FindLookAtRotation(camera->GetComponentLocation(), ptr->GetActorLocation());
			FRotator DifRotator = CameraToEnemy - CameraToPlayer;
			float DifValueOfZ = DifRotator.Yaw;
			TargetActorArray.Add(ptr, abs(DifValueOfZ));
		}
	}
}

void APlayerCharacter::InitSocketLocation()
{
	SocketLocation.Empty();
	SocketNames.Empty();

	UStaticMeshComponent* WeaponMesh = GetWeapon();
	SocketNames = WeaponMesh->GetAllSocketNames();
	for (int8 i = 0; i < SocketNames.Num(); ++i)
	{
		SocketLocation.AddUnique(WeaponMesh->GetSocketLocation(SocketNames[i]));
	}
}

void APlayerCharacter::EquipEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UWorld* world = GetWorld();
	EnableInput(UGameplayStatics::GetPlayerController(world, 0));
}

void APlayerCharacter::UnEquipEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UWorld* world = GetWorld();
	EnableInput(UGameplayStatics::GetPlayerController(world, 0));
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void APlayerCharacter::AttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimInstance->Montage_IsPlaying(UpperACKMontage) || AnimInstance->Montage_IsPlaying(AirDownACK_Montage))
		PlayerCurrentMode = EPlayerStateMode::EACK;
	else
		PlayerCurrentMode = EPlayerStateMode::EEQUIP;
	if(AnimInstance->Montage_IsPlaying(AirDownACK_Montage))
		CurrPlayerACKMode = EPlayerACKMode::EDownACKMode;
	bIsPlayACKSound = false;
	bCheckDownACK = false;
	bCanCombo = false;
	CurrPlayerACKMode = ACKModeTemp;
	TargetEnemy.Empty();
	bIsAttacking = false;
	CurrentSkillType = ESkillType::ENULL;
}

void APlayerCharacter::SaveMyGame()
{
	UMySaveGame* SaveGameIns = Cast<UMySaveGame>(UMySaveGame::GetSaveGameIns());
	if (SaveGameIns)
	{
			SaveGameIns->SavePlayerLocation(GetActorLocation());
			//此函数创建本地存档，SlotName为存档名,传入创建的USaveGame的实例对象。
			if (UGameplayStatics::SaveGameToSlot(SaveGameIns, TEXT("MySave"), 0))
			{
				DebugNS::MyDebug(3.f, "Save Game Success！");
			}
	}
}


FPlayerData::FPlayerData()
{
}
