// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemyController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NormalEnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "MyAISenseConfig_Sight.h"
#include "MyAISenseConfig_Hearing.h"
#include "SoulsLikeDemo/Common/GameDataType.h"
#include "Kismet/GameplayStatics.h"

ANormalEnemyController::ANormalEnemyController(FObjectInitializer const& object_initializer)
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAssert(TEXT("/Game/Enemy/AIBTree"));//找到编辑器中的行为树
	if (BehaviorTreeAssert.Succeeded())
		BehaviorTree = BehaviorTreeAssert.Object;//将在编辑器中找到的行为树分配给CPP变量
	BBComp = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this,TEXT("BBComp"));

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	//TestSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("TestSightConfig"));
	//////配置AIPerceptionConfig
	////SightConfig = NewObject<UMyAISenseConfig_Sight>();//创建对象
	////AIPerceptionComp->ConfigureSense(*SightConfig);
	////HearingConfig = NewObject<UMyAISenseConfig_Hearing>();//创建对象
	////AIPerceptionComp->ConfigureSense(*HearingConfig);
	//if (AIPerceptionComp)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("AIPC Is Valid"));
	//	TestSightConfig->SightRadius = 1000.f;
	//	TestSightConfig->LoseSightRadius = 2000.f;
	//	TestSightConfig->PeripheralVisionAngleDegrees = 60.f;
	//	TestSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	//	TestSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	//	TestSightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//	AIPerceptionComp->SetDominantSense(UAISenseConfig_Sight::StaticClass());//视觉为主导感官

	//	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ANormalEnemyController::OnTargetPerceptionUpdated);

	//	AIPerceptionComp->ConfigureSense(*TestSightConfig);
	//}
}

void ANormalEnemyController::OnPossess(APawn* InPawn)
{
	ANormalEnemyCharacter* AIC = Cast<ANormalEnemyCharacter>(InPawn);
	if (AIC == nullptr)
		return;

	Super::OnPossess(InPawn);

	//AIC->GetMyBlackBoard()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);//将行为树绑定的黑板赋值给CPP变量
	if (BBComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitBlackBoard"));
		BBComp->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));//将行为树绑定的黑板赋值给CPP变量
	}
}

void ANormalEnemyController::OnUnPossess()
{
	Super::OnUnPossess();
	if (BehaviorTreeComp)
		BehaviorTreeComp->StopTree(EBTStopMode::Safe);//停止行为树运行
}

void ANormalEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (BehaviorTreeComp)
		if (BehaviorTree)
		{
			RunBehaviorTree(BehaviorTree);
			//BehaviorTreeComp->StartTree(*BehaviorTree,EBTExecutionMode::Looped);//运行行为树
		}
}

UBlackboardComponent* ANormalEnemyController::GetBlackBoardComp()
{
	return this->BBComp;
}

void ANormalEnemyController::SetFocusIs()
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (player)
		SetFocus(player);
}

void ANormalEnemyController::SetFocusNot()
{
	ClearFocus(EAIFocusPriority::Gameplay);
}

//void ANormalEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
//{
//	if (Actor)
//	{
//		DebugNS::MyDebug(3.f, FString("Here!"));
//	}
//	else
//	{
//		DebugNS::MyDebug(3.f, FString("There!"));
//	}
//	if (Stimulus.IsValid())
//	{
//		FAISenseID Type = Stimulus.Type;
//		UAISenseConfig* TempSenseConfig = AIPerceptionComp->GetSenseConfig(Type);
//		if (UMyAISenseConfig_Sight* AISenseConfig_Sight = Cast<UMyAISenseConfig_Sight>(TempSenseConfig))
//		{
//			DebugNS::MyDebug(3.f, FString("See Actor :") + Actor->GetName());//当看见目标时
//				BBComp->SetValueAsObject("TargetActor", Actor);
//		}
//		else if (UMyAISenseConfig_Hearing* AISenseConfig_Hearing = Cast<UMyAISenseConfig_Hearing>(TempSenseConfig))
//		{
//			DebugNS::MyDebug(3.f, FString("Hear Actor :") + Actor->GetName());//当听到目标时
//				BBComp->SetValueAsObject("HearTarget", Actor);
//		}
//	}
//}


