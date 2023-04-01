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
#include "Sound/SoundCue.h"

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

}

// Called when the game starts or when spawned
void ANormalEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();

	player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ANormalEnemyCharacter::OnBoxOverlap);
	/*CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ANormalEnemyCharacter::OnBoxEndOverlap);*/

	AnimInstanceTemp = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	Set_NormalMode();

	AnimInstanceTemp->OnPlayMontageNotifyBegin.AddDynamic(this, &ANormalEnemyCharacter::WhenNotifyBegin);
	AnimInstanceTemp->OnPlayMontageNotifyEnd.AddDynamic(this, &ANormalEnemyCharacter::WhenNotifyEnd);

	UWorld* world = GetWorld();
	PlayerTemp = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(world, 0));

	CurrentDistance = EDistanceType::EFar;

	PawnSensingComp->OnHearNoise.Add(HearDelegate);

	AIC = Cast<ANormalEnemyController>(GetController());
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
}

// Called to bind functionality to input
void ANormalEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//void ANormalEnemyCharacter::OnMySeePawn(APawn* Pawn)
//{
//	ANormalEnemyController* AIC = Cast<ANormalEnemyController>(GetController());
//	APlayerCharacter* player = Cast<APlayerCharacter>(Pawn);
//	UBlackboardComponent* BBComp = AIC->GetBlackBoardComp();
//	if (Pawn && player)
//	{
//		if (BBComp)
//			BBComp->SetValueAsObject("TargetActor", Pawn);//设置黑板键值
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString(Pawn->GetName()));
//	}
//	/*if (Pawn)//导致不断执行Combat
//		Set_LockPlayerMode();*/
//}
//
//void ANormalEnemyCharacter::OnMyHearPawn(APawn* Pawn,const FVector& Location,float Volume)
//{
//	APlayerCharacter* player = Cast<APlayerCharacter>(Pawn);
//	AAIController* AIC = Cast<AAIController>(GetController());
//
//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Hear!"));
//
//	if (PawnSensingComp->CanHear(Location, Volume, false))
//	{
//		if (AIC)
//		{	
//			AIC->GetBlackboardComponent()->SetValueAsObject("HearTarget", player);
//			AIC->GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", true);
//		}
//	}
//}

void ANormalEnemyCharacter::SeePlayer()
{
	if (AIC && player)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackBoardComp();
		if(BBComp)
			BBComp->SetValueAsObject("TargetActor", player);//设置黑板键值
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
		//FRotator DifRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerCharacter(world, 0)->GetActorLocation());
		//float YawDif = DifRotator.Yaw - GetActorRotation().Yaw;
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString(FString::SanitizeFloat(YawDif)));
		//if ((YawDif >= 0 && YawDif < 60) || (YawDif > -60 && YawDif <= 0))
		//	playerCurrentDirect = EPlayerCurDirection::EBACK;
		//else if (YawDif >= 60 && YawDif < 105)
		//	playerCurrentDirect = EPlayerCurDirection::ELEFT;
		//else if (YawDif > -105 && YawDif <= -60)
		//	playerCurrentDirect = EPlayerCurDirection::ERIGHT;
		//else if (YawDif >= 105 && YawDif < 150)
		//	playerCurrentDirect = EPlayerCurDirection::ELFORWARD;
		//else if (YawDif > -150 && YawDif <= -105)
		//	playerCurrentDirect = EPlayerCurDirection::ERFORWARD;
		//else//150 ~ 0 ~ -150
		//	playerCurrentDirect = EPlayerCurDirection::EFORWARD;
		//switch (playerCurrentDirect)
		//{
		//case EPlayerCurDirection::EFORWARD:
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("ForWard"));
		//	break;
		//case EPlayerCurDirection::ELFORWARD:
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("LForWard"));
		//	break;
		//case EPlayerCurDirection::ELEFT:
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Left"));
		//	break;
		//case EPlayerCurDirection::ERFORWARD:
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("RForWard"));
		//	break;
		//case EPlayerCurDirection::ERIGHT:
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Right"));
		//	break;
		//case EPlayerCurDirection::EBACK:
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Back"));
		//	break;
		//default:
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("ErrorDirection"));
		//	break;
		//}
	}
}

void ANormalEnemyCharacter::SetHittedValue()
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

void ANormalEnemyCharacter::PlayHittedMontage(UAnimMontage* CurrentHittedMontage)
{
	UWorld* world = GetWorld();
	AnimInstanceTemp = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstanceTemp)
	{
		AIC->SetFocusNot();
		MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::HittedMontageEnded);
		AnimInstanceTemp->Montage_Play(CurrentHittedMontage);
		//SetHittedValue();
		AnimInstanceTemp->Montage_SetEndDelegate(MontageEndedDelegate);
	}
}

void ANormalEnemyCharacter::WhenNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UWorld* world = GetWorld();
	if (NotifyName == "HitBack1")
		GetWorldTimerManager().SetTimer(HitBackHandler, this, &ANormalEnemyCharacter::MakeHitBack, world->GetDeltaSeconds(), true);
	if (NotifyName == "AICombat")
		GetWorldTimerManager().SetTimer(CombatHandler, this, &ANormalEnemyCharacter::CheckACK, world->GetDeltaSeconds(), true);
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
			LaunchCharacter(FVector(0, 0, 1000.f), false, false);
			GetWorldTimerManager().SetTimer(SprintACKHandler, this, &ANormalEnemyCharacter::JumpAttackMovement, world->GetDeltaSeconds(), true);
		}
	}
}

void ANormalEnemyCharacter::WhenNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UWorld* world = GetWorld();
	if (NotifyName == "HitBack1")
		GetWorldTimerManager().ClearTimer(HitBackHandler);
	if (NotifyName == "AICombat")
		GetWorldTimerManager().ClearTimer(CombatHandler);
	if (NotifyName == "SprintACK")
	{
		GetWorldTimerManager().ClearTimer(SprintACKHandler);
		GetWorldTimerManager().ClearTimer(CombatHandler);
	}
	if (NotifyName == "JumpAttack")
	{
		//播放落地攻击的特效
		SpawnJumpAttackEffect();
		GetWorldTimerManager().ClearTimer(SprintACKHandler);
	}
}

void ANormalEnemyCharacter::MakeHitBack()
{
	UWorld* world = GetWorld();
	FVector TargetLocation;
	//用于防止与玩家距离过近情况
	if (player)
	{
		float dis = std::abs((player->GetActorLocation() - GetActorLocation()).Size());
		if (dis <= 50.f)
			TargetLocation = (-GetActorForwardVector()) * 40 + GetActorLocation();
		else
			TargetLocation = (-GetActorForwardVector()) * 20 + GetActorLocation();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("HitBack!"));
	FVector FinalLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, world->GetDeltaSeconds(), 20);
	SetActorLocation(FinalLocation);
}

void ANormalEnemyCharacter::Set_NormalMode()
{
	EnemyCurrentMode = ENormalEnemyMode::ENormal;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ANormalEnemyCharacter::Set_LockPlayerMode()
{
	EnemyCurrentMode = ENormalEnemyMode::ELockPlayer;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ANormalEnemyCharacter::Set_Attack()
{
	EnemyCurrentMode = ENormalEnemyMode::EAttack;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ANormalEnemyCharacter::PlayComboMontage()
{
	Set_Attack();
	MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::AttackMontageEnd);
	AnimInstance->Montage_Play(EnemyCombatMontage);
	MakeHitType = EMakeHitType::CommonHit;
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
	InitSocketLocation();
}

void ANormalEnemyCharacter::AttackMontageEnd(UAnimMontage* AnimMontage, bool bInterrupted)
{
	Set_LockPlayerMode();
}

ENormalEnemyMode ANormalEnemyCharacter::GetCurrentMode()
{
	return EnemyCurrentMode;
}

void ANormalEnemyCharacter::PrintCurrentMode()
{
	switch (EnemyCurrentMode)
	{
	case ENormalEnemyMode::ENormal:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("Normal"));
		break;
	case ENormalEnemyMode::ELockPlayer:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("LockPlayer"));
		break;
	case ENormalEnemyMode::EAttack:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString("AttackMode"));
		break;
	default:
		break;
	}
}

void ANormalEnemyCharacter::HittedMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AIC->SetFocusIs();
	SetHittedValue();
}

bool ANormalEnemyCharacter::GetHittedValue()
{
	return bBeHitted;
}

void ANormalEnemyCharacter::CheckACK()
{
	UWorld* world = GetWorld();
	TargetArr.Empty();
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
			if (player)
				TargetArr.AddUnique(player);
		}
		if(TargetArr.Num() != 0)
			for (auto Target : TargetArr)
			{
				player = Cast<APlayerCharacter>(Target);
				if (player)
				{
					switch (MakeHitType)
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
	MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::SprintMontageEnd);
	AnimInstance->Montage_Play(JumpACKMontage);
	MakeHitType = EMakeHitType::HitFly;
	AIC->SetFocusNot();
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
}

void ANormalEnemyCharacter::SprintMontageEnd(UAnimMontage* AnimMontage, bool bInterrupted)
{
	AIC->SetFocusIs();
	Set_LockPlayerMode();
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
	//DebugNS::DurationDebug(3.f, FString("Launch!"));
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
				SetHittedValue();
			}
		}
		else
		{
			AMissileActor* Missile = Cast<AMissileActor>(OtherActor);
			//被导弹打中
			if (Missile)
			{
				if (Missile->Destroy())
					SetHittedValue();
			}
		}
	}
}

void ANormalEnemyCharacter::AIDogeBack()
{
	if (ensure(AnimInstance) && DogeBackMontage)
	{
		AnimInstance->Montage_Play(DogeBackMontage);
		MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::SingleACKMontageEnd);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
	}
}

void ANormalEnemyCharacter::SetBTTDelay()
{
	if (bAnimDelayComplete)
		bAnimDelayComplete = false;
	else
	{
		//这一层判断防止出现在某些蒙太奇播放被中断也能设置布尔值为true的情况
		if(!GetMesh()->GetAnimInstance()->Montage_IsPlaying(JumpAttackMontage) && 
			!GetMesh()->GetAnimInstance()->Montage_IsPlaying(BladeLightAttackMontage) && 
			!GetMesh()->GetAnimInstance()->Montage_IsPlaying(DogeBackMontage))
			bAnimDelayComplete = true;
	}
}

bool ANormalEnemyCharacter::BTTDelayCheck()
{
	return bAnimDelayComplete;
}

void ANormalEnemyCharacter::ChancelDelayBool()
{
	SetBTTDelay();
}

void ANormalEnemyCharacter::SingleACKMontageEnd(UAnimMontage* AnimMontage, bool bInterrupted)
{
	AIC->SetFocusIs();
	SetBTTDelay();
	GetWorldTimerManager().SetTimer(BTTDelayHandler, this, &ANormalEnemyCharacter::ChancelDelayBool, 0.5f, false);
}

void ANormalEnemyCharacter::AIJumpAttack()
{
	if (ensure(AnimInstance) && JumpAttackMontage && AIC)
	{
		MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::SingleACKMontageEnd);
		AnimInstance->Montage_Play(JumpAttackMontage);
		MakeHitType = EMakeHitType::HitFly;
		//关闭对玩家的碰撞并且关闭Focus
		AIC->SetFocusNot();
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
	}
}

void ANormalEnemyCharacter::JumpAttackMovement()
{
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), PlayerCurrentLocation, GetWorld()->GetDeltaSeconds(), 20.f));
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
		MontageEndedDelegate.BindUObject(this, &ANormalEnemyCharacter::SingleACKMontageEnd);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
	}
}

void ANormalEnemyCharacter::TakeDamage(float DamageValue)
{
	//把HP值控制在0~100范围内
	HP = FMath::Clamp<float>(HP - DamageValue, 0.f, 100.f);
	//把AM值控制在0~100范围内
	//..

	TMap<AI_Option::Type, float> Result;
	Result.Add(AI_Option::Type::EHP, HP);
	Result.Add(AI_Option::Type::EAM, Armmor);

	//声明为静态可以只用声明一次，减少计算量
	static AMainHUD* MainHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if(MainHUD)
		if (MainHUD && MainHUD->GetEnemyWidget().IsValid())
		{
			//同理
			if(!EnemyWidget.IsValid())
				EnemyWidget = MainHUD->GetEnemyWidget();

			EnemyWidget->ChangeAI_OptionBar(Result);
		}

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




