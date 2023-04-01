// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_NextWayPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SoulsLikeDemo/ScenceActor/WalkFlagActor.h"



UBTT_NextWayPoint::UBTT_NextWayPoint()
{

}

EBTNodeResult::Type UBTT_NextWayPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
	if (BBC)
	{
		UObject* obj = BBC->GetValueAsObject("NextWayPoint");
		AWalkFlagActor* CurrWayPoint = Cast<AWalkFlagActor>(obj);
		AWalkFlagActor* NextPoint = CurrWayPoint->GetNextPoint();
		BBC->SetValueAsObject("NextWayPoint", NextPoint);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
