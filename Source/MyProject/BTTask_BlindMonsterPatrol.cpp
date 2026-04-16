// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BlindMonsterPatrol.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BlindMonsterPatrol::UBTTask_BlindMonsterPatrol()
{
	NodeName = "Update Patrol Location";
}

EBTNodeResult::Type UBTTask_BlindMonsterPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* const Pawn = AIController->GetPawn();
	UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (!NavSys || !Pawn) return EBTNodeResult::Failed;
	
	FVector NextLocation = Pawn->GetActorLocation();
	
	if (bUseRandomPatrol)
	{
		FNavLocation NavLocation;
		if (NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), SearchRadius, NavLocation))
		{
			NextLocation = NavLocation.Location;
		}
	}
	else if (SpecificPatrolPoints.Num() > 0)
	{
		int Index = FMath::RandRange(0, SpecificPatrolPoints.Num() - 1);
		NextLocation = SpecificPatrolPoints[Index];
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NextLocation);
	
	return EBTNodeResult::Succeeded;
}
