// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SoulsLikeDemo/PlayerCharacter/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NormalEnemyController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnemyAnimInstance.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "SoulsLikeDemo/Robot/BulletActor.h"
#include "Components/BoxComponent.h"
#include "SoulsLikeDemo/SkillActor/JumpAttackEffectActor.h"
#include "SoulsLikeDemo/Robot/MissileActor.h"
#include "SoulsLikeDemo/HUD/MainHUD.h"
#include "SoulsLikeDemo/Widget/SEnemyWidget.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Sound/SoundCue.h"
#include "SoulsLikeDemo/SkillActor/LightingCheckActor.h"
#include "SoulsLikeDemo/SkillActor/BladeLightActor.h"

// Sets default values
ANormalEnemyCharacter::ANormalEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh(), FName("HAIR"));
	LockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockMesh"));
	LockMesh->SetupAttachment(GetRootComponent());
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("RHandWeapon"));

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	
	PawnSensingComp->bSeePawns = true;				//设置是否能看到玩家
	PawnSensingComp->SetPeripheralVisionAngle(60);  //设置视野角度为60
	PawnSensingComp->SightRadius = 1000.0f;			//设置视野半径为10m
	PawnSensingComp->bOnlySensePlayers = true;		//仅仅能感知到玩家
	
	PawnSensingComp->HearingThreshold = 100.0f;		//设置最小听力阈值
	PawnSensingComp->LOSHearingThreshold = 2000.0f; //设置最大听力阈值
	PawnSensingComp->bHearNoises = true;			//设置能否听见噪音
	PawnSensingComp->HearingMaxSoundAge = 1.0f;		//设置最大听觉年龄

	PawnSensingComp->SetSensingInterval(0.5f);		//设置每0.5s调用一次回调
	HearDelegate.BindUFunction(this, "OnMyHearPawn");

	//if (PawnSensingComp)
	//PawnSensingComp->OnSeePawn.AddDynamic(this, &ANormalEnemyCharacter::OnMySeePawn);

	AIControllerClass = ANormalEnemyController::StaticClass();

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(34.f, 34.f, 88.f));

	HP = 100.f;
	Armmor = 100.f;

	AiHittedType = EHittedType::None;
	MakeHitType = EMakeHitType::CommonHit;
	BodyState = EBossStateType::BT;
	DeBuffState = EBossStateType::NONE;

	CurrentMontage = nullptr;
}

// Called when the game starts or when spawned
void ANormalEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();

	player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ANormalEnemyCharacter::OnBoxOverlap);

	AnimInstanceTemp = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstanceTemp->OnPlayMontageNotifyBegin.AddDynamic(this, &ANormalEnemyCharacter::WhenNotifyBegin);
	AnimInstanceTemp->OnPlayMontageNotifyEnd.AddDynamic(this, &ANormalEnemyCharacter::WhenNotifyEnd);

	UWorld* world = GetWorld();
	PlayerTemp = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(world, 0));

	CurrentDistance = EDistanceType::EFar;

	PawnSensingComp->OnHearNoise.Add(HearDelegate);

	AIC = Cast<ANormalEnemyController>(GetController());
	//初始化为霸体状态
	if (AIC && AIC->GetBlackboardComponent())
		AIC->GetBlackboardComponent()->SetValueAsEnum("BossState", 0);
		
}

//Character类用于初始化Components函数在play期间调用
void ANormalEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ANormalEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//PrintCurrentMode();
	//GetDIfRotation();
	//PrintDistance();
	//DisplayDIstanceType();
	HealEnergyValue();
	//DisplayFoucus();
	//DisplayHitted();
	//DisplayState();
}

// Called to bind functionality to input
void ANormalEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANormalEnemyCharacter::SeePlayer()
{
	if (AIC && player)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackBoardComp();
		if (BBComp)
		{
			BBComp->SetValueAsObject("TargetActor", player);//设置黑板键值
			AIC->SetFocusIs();
		}
	}
}

void ANormalEnemyCharacter::SetLockMeshVisible()
{
	if (LockMesh->IsVisible())
		LockMesh->SetVisibility(false);
	else
		LockMesh->SetVisibility(true);
}

UStaticMeshComponent* ANormalEnemyCharacter::GetLockMesh()
{
	return LockMesh;
}

void ANormalEnemyCharacter::GetDIfRotation()
{
	UWorld* world = GetWorld();
	if (UGameplayStatics::GetPlayerCharacter(world, 0))
	{
		float DotValue = FVector::DotProduct(GetActorForwardVector(), player->GetActorForwardVector());
		float AngleValue = acos((DotValue) / (GetActorForwardVector().Size() * player->GetActorForwardVector().Size()));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString(FString::SanitizeFloat(AngleValue)));
	}
}

void ANormalEnemyCharacter::SetHittedValue(bool value)
{
	if (AIC)
	{
		UBlackboardComponent* BBoard = AIC->GetBlackBoardComp();
		uint8 result = BBoard->GetValueAsEnum("BossState");
		if (result != 0 && BBoard && !AnimInstance->Montage_IsPlaying(HitDownMontage) && !AnimInstance->Montage_IsPlaying(HitFlyMontage))
		{
			if (bBeHitted && BBoard && value)
			{
				bHitAgain = true;
				GetWorldTimerManager().SetTimer(SwitchHittedHandler, this, &ANormalEnemyCharacter::HittedValueDelay, 0.1f, false);
			}
			if (value)
				bBeHitted = true;
			else
				bBeHitted = false;

			BBoard->SetValueAsBool("BeHitted", bBeHitted);
		}
	}
}

void ANormalEnemyCharacter::PlayHittedMontage(UAnimMontage* CurrentHittedMontage)
{
	UWorld* world = GetWorld();
	AnimInstanceTemp = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstanceTemp)
	{
		if (CurrentMontage && AnimInstanceTemp->Montage_IsPlaying(CurrentMontage))
			AnimInstanceTemp->Montage_Stop(0.f,CurrentMontage);
		MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::HittedMontageEnded);
		AnimInstanceTemp->Montage_Play(CurrentHittedMontage);
		AnimInstanceTemp->Montage_SetEndDelegate(MontageEndedDelegate);
	}
}

void ANormalEnemyCharacter::WhenNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UWorld* world = GetWorld();
	if (NotifyName == "HitBack1")
	{
		GetWorldTimerManager().SetTimer(HitBackHandler, this, &ANormalEnemyCharacter::MakeHitBack, world->GetDeltaSeconds(), true);
	}
	if (NotifyName == "AICombat")
	{
		InitSocketLocation();
		GetWorldTimerManager().SetTimer(CombatHandler, this, &ANormalEnemyCharacter::CheckACK, world->GetDeltaSeconds(), true);
	}
	if (NotifyName == "Slide1" || NotifyName == "Slide2" || NotifyName == "Slide3")
		if(BoxTraceCheckPlayer(80.f, FVector(50.f, 80.f, 30.f)))	//这一层判断用于检测玩家是否在AI周围，如果不在那么近身攻击极限闪避也就没意义了
			PlayerTemp->CheckPerfectSlide();
	if (NotifyName == "SprintACK")
	{
		if (player)
		{
			GetWorldTimerManager().ClearTimer(SprintACKHandler);
			GetWorldTimerManager().ClearTimer(CombatHandler);
			FVector DisVector = GetActorLocation() - player->GetActorLocation();
			if (DisVector.Normalize())
				PlayerCurrentLocation = player->GetActorLocation() + DisVector * 150.f;
			InitSocketLocation();
			GetWorldTimerManager().SetTimer(SprintACKHandler, this, &ANormalEnemyCharacter::SprintMovement, world->GetDeltaSeconds(), true);
			GetWorldTimerManager().SetTimer(CombatHandler, this, &ANormalEnemyCharacter::CheckACK, world->GetDeltaSeconds(), true);
		}
	}
	if (NotifyName == "JumpAttack")
	{
		if (player)
		{
			GetWorldTimerManager().ClearTimer(SprintACKHandler);
			//在通知一开始的时候给予一个Z轴的速度，并且记录AI起跳时玩家的位置
			FVector DisVector = GetActorLocation() - player->GetActorLocation();
			if(DisVector.Normalize())
				PlayerCurrentLocation = player->GetActorLocation() + DisVector * 150.f;
			PlayerCurrentLocation = FVector(PlayerCurrentLocation.X, PlayerCurrentLocation.Y, 30.f);
			LaunchCharacter(FVector(0, 0, 1000.f), false, false);
			GetWorldTimerManager().SetTimer(SprintACKHandler, this, &ANormalEnemyCharacter::JumpAttackMovement, world->GetDeltaSeconds(), true);
		}
	}
	if (NotifyName == "AIHitFly")
	{
		GetWorldTimerManager().ClearTimer(HitBackHandler);
		GetWorldTimerManager().SetTimer(HitBackHandler, this, &ANormalEnemyCharacter::AIHitFly, GetWorld()->DeltaTimeSeconds, true);
	}
}

void ANormalEnemyCharacter::WhenNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UWorld* world = GetWorld();
	if (NotifyName == "HitBack1")
		GetWorldTimerManager().ClearTimer(HitBackHandler);
	if (NotifyName == "AICombat")
	{
		GetWorldTimerManager().ClearTimer(CombatHandler);
		TargetArr.Empty();
	}
	if (NotifyName == "SprintACK")
	{
		GetWorldTimerManager().ClearTimer(SprintACKHandler);
		GetWorldTimerManager().ClearTimer(CombatHandler);
		TargetArr.Empty();
	}
	if (NotifyName == "JumpAttack")
	{
		//播放落地攻击的特效
		SpawnJumpAttackEffect();
		GetWorldTimerManager().ClearTimer(SprintACKHandler);
		TargetArr.Empty();
	}
	if (NotifyName == "AIHitFly")
	{
		GetWorldTimerManager().ClearTimer(HitBackHandler);
	}
}

void ANormalEnemyCharacter::MakeHitBack()
{
	FVector TargetLocation = GetActorLocation() - GetActorForwardVector() * 40.f;
	FVector FinalLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), 20);
	SetActorLocation(FinalLocation);
}

void ANormalEnemyCharacter::PlayComboMontage()
{
	MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::CurrMontageOver);
	AnimInstance->Montage_Play(EnemyCombatMontage);
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
	CurrentMontage = EnemyCombatMontage;
	MakeHitType = EMakeHitType::CommonHit;
	AIC->SetFocusNot();
	bIsFocus = false;
	AIC->GetBlackboardComponent()->SetValueAsBool("IsFocus", bIsFocus);
}

void ANormalEnemyCharacter::HittedMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetHittedValue(false);
	//OldSetHittedValue();
}

bool ANormalEnemyCharacter::GetHittedValue()
{
	return bBeHitted;
}

void ANormalEnemyCharacter::CheckACK()
{
	UWorld* world = GetWorld();
	TArray<AActor*> ActorsToIngore;
	TArray<FHitResult> HitResult;

	TargetTypes.AddUnique(EObjectTypeQuery::ObjectTypeQuery3);
	for (int i = 0 ; i<SocketLocationArr.Num() ; ++i)
	{
		if (UKismetSystemLibrary::LineTraceMultiForObjects(world, SocketLocationArr[i], WeaponMesh->GetSocketLocation(SocketNameArr[i]), TargetTypes, false, ActorsToIngore, EDrawDebugTrace::None, HitResult, true))
			AddTarget(HitResult);
	}
	InitSocketLocation();
}

void ANormalEnemyCharacter::InitSocketLocation()
{
	SocketLocationArr.Empty();
	SocketNameArr.Empty();
	SocketNameArr = WeaponMesh->GetAllSocketNames();
	for (const auto& SocketName : SocketNameArr)
		SocketLocationArr.AddUnique(WeaponMesh->GetSocketLocation(SocketName));
}

void ANormalEnemyCharacter::AddTarget(TArray<FHitResult>& HitResult)
{
	if (HitResult.Num() != 0)
	{
		for (auto& value : HitResult)
		{
			player = Cast<APlayerCharacter>(value.GetActor());
			if (player && !TargetArr.Contains(player))
			{
				TargetArr.AddUnique(player);
				SetPlayerHittedMontage(MakeHitType);
			}
		}
	}
		
}

EDistanceType& ANormalEnemyCharacter::GetCurrDistanceType()
{
	return CurrentDistance;
}

void ANormalEnemyCharacter::PrintDistance()
{
	switch (CurrentDistance)
	{
	case EDistanceType::EFar:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Far"));
		break;
	case EDistanceType::EMid:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Mid"));
		break;
	case EDistanceType::ENear:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Near"));
		break;
	case EDistanceType::EClose:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Close"));
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("ErrorDistance"));
		break;
	}
}

void ANormalEnemyCharacter::PlaySprintACKMontage()
{
	MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::CurrMontageOver);
	AnimInstance->Montage_Play(JumpACKMontage);
	CurrentMontage = JumpACKMontage;
	MakeHitType = EMakeHitType::HitFly;
	AIC->SetFocusNot();
	bIsFocus = false;
	AIC->GetBlackboardComponent()->SetValueAsBool("IsFocus", bIsFocus);
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
}

void ANormalEnemyCharacter::SprintMovement()
{
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), PlayerCurrentLocation, GetWorld()->GetDeltaSeconds(), 10.f));
}

void ANormalEnemyCharacter::UpperAI()
{
	LaunchCharacter(FVector(0.f, 0.f, 700.f), false, false);
	//在launch的一瞬间是没法检测到AI在空中的所以需要加定时器延迟一点时间，时间为0.1s。
	GetWorldTimerManager().SetTimer(DelayHandler, this, &ANormalEnemyCharacter::CancelDelayHandler, 0.1, false);
}

void ANormalEnemyCharacter::UpperEndAI()
{
	TimeCounter += GetWorld()->DeltaTimeSeconds;
	if (TimeCounter <= 2.f)
		SetActorLocation(player->GetActorLocation() + player->GetActorForwardVector() * 100.f);
	if (!AIIsInAir())
	{
		if (AIC)
		{
			UBlackboardComponent* BBComp = AIC->GetBlackBoardComp();
			//将值设置给黑板
			if (BBComp)
			{
				BBComp->SetValueAsBool(FName("IsInAir"), false);
				GetWorldTimerManager().ClearTimer(UpperHandler);
			}
		}
	}
}

bool ANormalEnemyCharacter::AIIsInAir()
{
	return GetCharacterMovement()->IsFalling();
}

void ANormalEnemyCharacter::CancelDelayHandler()
{
	GetWorldTimerManager().ClearTimer(DelayHandler);
	SetActorLocation(player->GetActorLocation() + player->GetActorForwardVector() * 100.f);
	if (AIIsInAir())
	{
		if (AIC)
		{
			UBlackboardComponent* BBComp = AIC->GetBlackBoardComp();
			//将值设置给黑板
			if (BBComp)
				BBComp->SetValueAsBool(FName("IsInAir"), true);
		}
		TimeCounter = 0.f;
		GetWorldTimerManager().SetTimer(UpperHandler, this, &ANormalEnemyCharacter::UpperEndAI, GetWorld()->GetDeltaSeconds(), true);
	}
}

void ANormalEnemyCharacter::BeAirCombEffect()
{
	LaunchCharacter(FVector(0.f, 0.f, 400.f), false, true);
}

void ANormalEnemyCharacter::AirHittedReSetLocation(float value)
{

	float X = GetActorLocation().X;
	float Y = GetActorLocation().Y;
	float Z = player->GetActorLocation().Z;
	FVector NewLocation(X, Y, Z);
	//NewLocation = NewLocation + GetActorForwardVector() * value;

	NewLocation = player->GetActorLocation() + player->GetActorForwardVector() * value;

	SetActorLocation(NewLocation, true);
}

//用于测试完美闪避
bool ANormalEnemyCharacter::BoxTraceCheckPlayer(float value, FVector HalfSize)
{
	//DebugNS::MyDebug(3.f, FString("Checking Player!"));
	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = GetActorLocation() + GetActorForwardVector() * value;
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.AddUnique(EObjectTypeQuery::ObjectTypeQuery3);
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;

	if (UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, HalfSize, GetActorRotation(), ObjectTypes, false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResult, true))
	{
		//DebugNS::MyDebug(3.f, FString("HasHittedPlayer!"));
		player = Cast<APlayerCharacter>(HitResult.GetActor());
		return true;
	}
	else
	{
		//DebugNS::MyDebug(3.f, FString("NotHittedPlayer!"));
		return false;
	}
}

void ANormalEnemyCharacter::SpawnLightingCheckBox()
{
	USoundCue* LightingCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/MedievalCombatSounds/MedievalCombatSounds/Magic/wav/LightingACKCue.LightingACKCue"));
	if (LightingCue)
		UGameplayStatics::PlaySound2D(GetWorld(), LightingCue);
	FVector SpawnLocation = (GetActorLocation() + UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()) / 2;
	ALightingCheckActor* DamageBox = GetWorld()->SpawnActor<ALightingCheckActor>(ALightingCheckActor::StaticClass(), SpawnLocation, GetActorRotation());
	FTimerDelegate LightingDelegate;
	FTimerDelegate LightingDamageDelegate;
	LightingDelegate.BindUObject(this, &ANormalEnemyCharacter::LightingACKOver, DamageBox);
	LightingDamageDelegate.BindUObject(this, &ANormalEnemyCharacter::LightingACK, DamageBox);
	bIsLightingACK = true;
	GetWorldTimerManager().SetTimer(LightingACKHandler, LightingDelegate, 1.5f, false);
	GetWorldTimerManager().SetTimer(IsLightingACKHandler, LightingDamageDelegate, 0.3f, true);
}

void ANormalEnemyCharacter::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABulletActor* Bullet = Cast<ABulletActor>(OtherActor);
		//被子弹打中
		if (Bullet)
		{
			if (Bullet->Destroy())
			{
				USoundCue* BulletHittedSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/MySound/BulHitted.BulHitted"));
				if (BulletHittedSound)
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletHittedSound, GetActorLocation());
				TakeDamageAndAmmor(1.f);
			
				SetHittedValue(true);
			}
		}
		else
		{
			AMissileActor* Missile = Cast<AMissileActor>(OtherActor);
			//被导弹打中
			if (Missile)
			{
				if (Missile->Destroy())
					SetHittedValue(true);
			}
		}
	}
}

void ANormalEnemyCharacter::AIDogeBack()
{
	if (ensure(AnimInstance) && DogeBackMontage)
	{
		AnimInstance->Montage_Play(DogeBackMontage);
		CurrentMontage = DogeBackMontage;
		MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::SingleACKMontageEnd);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
	}
}

bool ANormalEnemyCharacter::BTTDelayCheck()
{
	return bAnimDelayComplete;
}

void ANormalEnemyCharacter::SingleACKMontageEnd(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bCurrMontagePlay = false;
	AIC->SetFocusIs();
	bIsFocus = true;
	AIC->GetBlackboardComponent()->SetValueAsBool("IsFocus", bIsFocus);
}

void ANormalEnemyCharacter::AIJumpAttack()
{
	if (ensure(AnimInstance) && JumpAttackMontage && AIC)
	{
		MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::SingleACKMontageEnd);
		AnimInstance->Montage_Play(JumpAttackMontage);
		//关闭对玩家的碰撞并且关闭Focus
		AIC->SetFocusNot();
		bIsFocus = false;
		AIC->GetBlackboardComponent()->SetValueAsBool("IsFocus", bIsFocus);
		CurrentMontage = JumpAttackMontage;
		MakeHitType = EMakeHitType::HitFly;
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
	}
}

void ANormalEnemyCharacter::JumpAttackMovement()
{
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), PlayerCurrentLocation, GetWorld()->GetDeltaSeconds(), 10.f));
}

void ANormalEnemyCharacter::SpawnJumpAttackEffect()
{
	UWorld* world = GetWorld();
	FVector HitVector = GetActorLocation();
	world->SpawnActor<AJumpAttackEffectActor>(AJumpAttackEffectActor::StaticClass(), HitVector, FRotator(0.0f));
}

void ANormalEnemyCharacter::AIBladeLightAttack()
{
	if (ensure(AnimInstance) && BladeLightAttackMontage)
	{
		AnimInstance->Montage_Play(BladeLightAttackMontage);
		CurrentMontage = BladeLightAttackMontage;
		MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::SingleACKMontageEnd);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
	}
}

void ANormalEnemyCharacter::TakeDamageAndAmmor(float DamageValue)
{
	if (!bDied)
	{
		//声明为静态可以只用声明一次，减少计算量
		static AMainHUD* MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		//把HP值控制在0~100范围内
		HP = FMath::Clamp<float>(HP - DamageValue / 2.f, 0.f, 100.f);
		//把AM值控制在0~100范围内
		if (Armmor != 0)
		{
			Armmor = FMath::Clamp<float>(Armmor - 3 * DamageValue, 0.f, 100.f);
			if (Armmor == 0 && AIC)
			{
				BodyState = EBossStateType::QJ;
				AIC->GetBlackboardComponent()->SetValueAsEnum("BossState", 1);
				if (MainHUD->GetEnemyWidget().IsValid())
					MainHUD->GetEnemyWidget()->ChangeAI_State(EBossStateType::QJ);
				bArmmorEmpty = true;
				GetWorldTimerManager().SetTimer(EnergyEmptyHandler, this, &ANormalEnemyCharacter::CannelEnergyEmpty, 20.f, false);
			}
		}

		if (MainHUD)
			if (MainHUD && MainHUD->GetEnemyWidget().IsValid())
			{
				TMap<AI_Option::Type, float> Result;
				Result.Add(AI_Option::Type::EHP, HP);
				Result.Add(AI_Option::Type::EAM, Armmor);
				//同理
				if (!EnemyWidget.IsValid())
					EnemyWidget = MainHUD->GetEnemyWidget();

				EnemyWidget->ChangeAI_OptionBar(Result);
			}
		//血量已空，AI死亡
		if (HP - 0.f <= 0.001 && AIC)
		{
			BodyState = EBossStateType::DIE;
			AIC->GetBlackboardComponent()->SetValueAsEnum("BossState", 2);
			return;
		}
	}
}

void ANormalEnemyCharacter::CannelEnergyEmpty()
{
	GetWorldTimerManager().ClearTimer(EnergyEmptyHandler);
	bArmmorEmpty = false;
}

void ANormalEnemyCharacter::HealEnergyValue()
{
	if (!bIsBurnning)
	{
		//声明为静态可以只用声明一次，减少计算量
		static AMainHUD* MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		if (!bArmmorEmpty && BodyState == EBossStateType::QJ && AIC)
		{
			SetHittedValue(false);
			BodyState = EBossStateType::BT;
			AIC->GetBlackboardComponent()->SetValueAsEnum("BossState", 0);
			Armmor = 100;
			if (MainHUD->GetEnemyWidget().IsValid())
			{
				if (!EnemyWidget.IsValid())
					EnemyWidget = MainHUD->GetEnemyWidget();
				if (EnemyWidget.IsValid())
				{
					TMap<AI_Option::Type, float> Result;
					Result.Add(AI_Option::Type::EAM, Armmor);
					EnemyWidget->ChangeAI_OptionBar(Result);
					EnemyWidget->ChangeAI_State(EBossStateType::BT);
				}
			}
		}
		else if (!bArmmorEmpty && BodyState == EBossStateType::BT && Armmor < 100)
		{
			Armmor = FMath::Clamp<float>(Armmor + 0.01, 0, 100);

			if (MainHUD->GetEnemyWidget().IsValid())
			{
				if (!EnemyWidget.IsValid())
					EnemyWidget = MainHUD->GetEnemyWidget();
				if (EnemyWidget.IsValid())
				{
					TMap<AI_Option::Type, float> Result;
					Result.Add(AI_Option::Type::EAM, Armmor);
					EnemyWidget->ChangeAI_OptionBar(Result);
				}
			}
		}
	}
}

void ANormalEnemyCharacter::SetBurnEffect()
{
	bIsBurnning = true;
	static AMainHUD* MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	DeBuffState = EBossStateType::RS;
	if (MainHUD->GetEnemyWidget().IsValid())
		MainHUD->GetEnemyWidget()->ChangeAI_State(EBossStateType::RS);
	FTimerDelegate BurnOverDelegate;
	BurnOverDelegate.BindUObject(this, &ANormalEnemyCharacter::BurnOver, MainHUD);
	GetWorldTimerManager().SetTimer(BurnCounterHandler, BurnOverDelegate, 8.f, false);
	GetWorldTimerManager().SetTimer(BurnningHandler, this, &ANormalEnemyCharacter::IsBurning, 0.4f, true);
}

void ANormalEnemyCharacter::IsBurning()
{
	TakeDamageAndAmmor(1.f);
}

void ANormalEnemyCharacter::BurnOver(AMainHUD* Hud)
{
	GetWorldTimerManager().ClearTimer(BurnCounterHandler);
	GetWorldTimerManager().ClearTimer(BurnningHandler);
	bIsBurnning = false;
	DeBuffState = EBossStateType::NONE;
	if (Hud->GetEnemyWidget().IsValid())
	{
		Hud->GetEnemyWidget()->SetDebuffNone(EBossStateType::RS);
	}

}

void ANormalEnemyCharacter::SetLightingEffect()
{
	bIsLighting = true;
	static AMainHUD* MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	DeBuffState = EBossStateType::LJ;
	if (MainHUD->GetEnemyWidget().IsValid())
		MainHUD->GetEnemyWidget()->ChangeAI_State(EBossStateType::LJ);
	FTimerDelegate LightingOverDelegate;
	LightingOverDelegate.BindUObject(this, &ANormalEnemyCharacter::LightingOver, MainHUD);
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetWorldTimerManager().SetTimer(LightingCounterHandler, LightingOverDelegate, 8.f, false);
	GetWorldTimerManager().SetTimer(LightingHandler, this,&ANormalEnemyCharacter::IsLighting, GetWorld()->DeltaTimeSeconds, true);
}

void ANormalEnemyCharacter::IsLighting()
{
	if (AnimInstance && CurrentMontage && AnimInstance->Montage_IsPlaying(CurrentMontage))
		AnimInstance->Montage_SetPlayRate(CurrentMontage,0.5f);
}

void ANormalEnemyCharacter::LightingOver(AMainHUD* Hud)
{
	GetWorldTimerManager().ClearTimer(LightingCounterHandler);
	GetWorldTimerManager().ClearTimer(LightingHandler);
	if (AnimInstance && CurrentMontage && AnimInstance->Montage_IsPlaying(CurrentMontage))
		AnimInstance->Montage_SetPlayRate(CurrentMontage,1.f);
	if (Hud->GetEnemyWidget().IsValid())
		Hud->GetEnemyWidget()->SetDebuffNone(EBossStateType::LJ);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	bIsLighting = false;
	DeBuffState = EBossStateType::NONE;
}

void ANormalEnemyCharacter::CurrMontageOver(UAnimMontage* Montage, bool bInterrupted)
{
	AIC->SetFocusIs();
	bIsFocus = true;
	AIC->GetBlackboardComponent()->SetValueAsBool("IsFocus", bIsFocus);
	CurrentMontage = nullptr;
	bCurrMontagePlay = false;
}

void ANormalEnemyCharacter::LightingACK(ALightingCheckActor* actor)
{
	if (actor->IsCheckPlayer())
		SetPlayerHittedMontage(EMakeHitType::CommonHit);
}

void ANormalEnemyCharacter::LightingACKOver(ALightingCheckActor* actor)
{
	GetWorldTimerManager().ClearTimer(IsLightingACKHandler);
	GetWorldTimerManager().ClearTimer(LightingACKHandler);
	bIsLightingACK = false;
	actor->Destroy();
}

void ANormalEnemyCharacter::BossDied()
{
	bDied = true;
	if (AnimInstance && AnimInstance->Montage_IsPlaying(CurrentMontage))
		AnimInstance->Montage_Stop(0.f, CurrentMontage);
	if (BossLossMontage && AIC)
	{
		AIC->SetFocusNot();
		bIsFocus = false;
		AIC->GetBlackboardComponent()->SetValueAsBool("IsFocus", bIsFocus);
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		SetActorLocation(GetActorLocation());
		AnimInstance->Montage_Play(BossLossMontage);
	}
}

void ANormalEnemyCharacter::SetFocusBoolean(bool value)
{
	if (value)
		bIsFocus = true;
	else
		bIsFocus = false;
}

void ANormalEnemyCharacter::DisplayFoucus()
{
	if(bIsFocus)
		DebugNS::DurationDebug(1.f, "Focus");
	else
		DebugNS::DurationDebug(1.f, "NotFocus");
}

void ANormalEnemyCharacter::DisplayHitted()
{
	if (AIC && AIC->GetBlackboardComponent())
	{
		bool result = AIC->GetBlackboardComponent()->GetValueAsBool("BeHitted");
		if (result)
			DebugNS::DurationDebug(1.f, "Hitted");
		else
			DebugNS::DurationDebug(1.f, "NotHitted");
	}
}

void ANormalEnemyCharacter::DisplayState()
{
	if (AIC && AIC->GetBlackboardComponent())
	{
		uint8 result = AIC->GetBlackboardComponent()->GetValueAsEnum("BossState");
		if (result == 0)
			DebugNS::DurationDebug(1.f, "BT");
		else if (result == 1)
			DebugNS::DurationDebug(1.f, "QJ");
	}
}

EBossStateType ANormalEnemyCharacter::GetBodyState()
{
	return BodyState;
}

bool ANormalEnemyCharacter::GetLightingACKBoolean()
{
	return bIsLightingACK;
}

void ANormalEnemyCharacter::SetPlayerHittedMontage(EMakeHitType HitType)
{
	if (player)
	{
		player->TakeHealth(5.f);
		if (!player->PlayerIsInAir())
		{
			switch (HitType)
			{
			case EMakeHitType::CommonHit:
				player->PlayHittedMontage(EPlayerHittedType::CommonHit);
				break;
			case EMakeHitType::HitFly:
				player->PlayHittedMontage(EPlayerHittedType::HitFly);
				break;
			default:
				DebugNS::MyDebug(1.f, "ErrorAIMakeHitType");
				break;
			}
		}
	}
}

void ANormalEnemyCharacter::SetInplayHittedMontageBoolean(bool value)
{
	if (value)
		bIsPlayHittedMontage = true;
	else
		bIsPlayHittedMontage = false;
}

bool ANormalEnemyCharacter::GetplayHittedMontageBoolean()
{
	return bIsPlayHittedMontage;
}

void ANormalEnemyCharacter::HittedValueDelay()
{
	GetWorldTimerManager().ClearTimer(SwitchHittedHandler);
	bHitAgain = false;
}

void ANormalEnemyCharacter::OldSetHittedValue()
{
		if (bBeHitted)
			bBeHitted = false;
		else
		{
			bBeHitted = true;
		}
		ANormalEnemyController* AIController = Cast<ANormalEnemyController>(GetController());
		if (AIController)
		{
			UBlackboardComponent* BBoard = AIController->GetBlackBoardComp();
			if (BBoard)
				BBoard->SetValueAsBool("BeHitted", bBeHitted);
		}
}

bool ANormalEnemyCharacter::GetHitAgainBoolean()
{
	return bHitAgain;
}


void ANormalEnemyCharacter::SetHittedType(FString HitType)
{
	if (HitType == "ComboHit")
		AiHittedType = EHittedType::ComboHit;
	else if (HitType == "ExcuteHit")
		AiHittedType = EHittedType::Excute;
	else if (HitType == "HitDown")
		AiHittedType = EHittedType::HitDown;
	else if (HitType == "HitFly")
		AiHittedType = EHittedType::HitFly;
	else if (HitType == "")
		AiHittedType = EHittedType::None;
	else
		DebugNS::MyDebug(1.f, "HitType Error!");
}

UAnimMontage* ANormalEnemyCharacter::GetHittedMontage()
{
	static UAnimMontage* CurrentHittedMontage = nullptr;

	switch (AiHittedType)
	{
	case EHittedType::None:
		CurrentHittedMontage = nullptr;
		break;
	case EHittedType::Excute:
		CurrentHittedMontage = ExcuteMontage;
		break;
	case EHittedType::ComboHit:
		CurrentHittedMontage = EnemyHitedMontage;
		break;
	case EHittedType::HitDown:
		CurrentHittedMontage = HitDownMontage;
		break;
	case EHittedType::HitFly:
		CurrentHittedMontage = HitFlyMontage;
		break;
	default:
		DebugNS::MyDebug(1.f, "ErrorHittedMontage!");
		break;
	}
	return CurrentHittedMontage;
}

float ANormalEnemyCharacter::GetDisWithVector(FVector& TargetLocation)
{
	float result = std::abs((GetActorLocation() - TargetLocation).Size());
	return result;
}

void ANormalEnemyCharacter::SlowEnemyAction()
{
	if (AnimInstance && AIC && AIC->GetBlackBoardComp())
	{
		AnimInstance->Montage_SetPlayRate(EnemyCombatMontage, 0.3);
		bIsSlowAction = true;
		AIC->GetBlackBoardComp()->SetValueAsBool(FName("IsSlowAction"), bIsSlowAction);
		GetWorldTimerManager().SetTimer(SlowMonatgeHandler, this, &ANormalEnemyCharacter::SlowActionOver, 2.f, false);
	}
}

void ANormalEnemyCharacter::SlowActionOver()
{
	GetWorldTimerManager().ClearTimer(SlowMonatgeHandler);
	if (AnimInstance && AIC && AIC->GetBlackBoardComp())
	{
		if (bBeHitted == true)
			{
				bBeHitted = false;
				AIC->GetBlackBoardComp()->SetValueAsBool(FName("BeHitted"), bBeHitted);
			}
		AnimInstance->Montage_SetPlayRate(EnemyCombatMontage, 1.f);
		bIsSlowAction = false;
		AIC->GetBlackBoardComp()->SetValueAsBool(FName("IsSlowAction"), bIsSlowAction);
	}
}

void ANormalEnemyCharacter::DisplayDIstanceType()
{
	if (AIC && AIC->GetBlackboardComponent())
	{
		DebugNS::MyDebug(0.5f,FString::SanitizeFloat(AIC->GetBlackboardComponent()->GetValueAsEnum("DIstanceType")));
	}
}

void ANormalEnemyCharacter::AIHitFly()
{
	FVector FinalLocation = GetActorLocation() + (-GetActorForwardVector() * 50);
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), FinalLocation, GetWorld()->DeltaTimeSeconds, 10.f));
}

FBossDamage::FBossDamage()
{
}
