// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "NormalEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKEDEMO_API ANormalEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	ANormalEnemyController(FObjectInitializer const& object_initializer);
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
protected:
	virtual void BeginPlay() override;

	class UBehaviorTreeComponent* BehaviorTreeComp;
	class UBehaviorTree* BehaviorTree;
	class UBlackboardComponent* BBComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAIPerceptionComponent* AIPerceptionComp;
	////AI感官配置
	//UPROPERTY()
	//class UMyAISenseConfig_Sight* SightConfig;
	//UPROPERTY()
	//class UMyAISenseConfig_Hearing* HearingConfig;
	//UPROPERTY()
	//	class UAISenseConfig_Sight* TestSightConfig;
public:
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComp() {
		return BehaviorTreeComp;
	}

		UBlackboardComponent* GetBlackBoardComp();

		void SetFocusIs();
		void SetFocusNot();


		/*void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);*/
};
