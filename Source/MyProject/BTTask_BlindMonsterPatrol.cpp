// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BlindMonsterPatrol.h"
#include "AIController.h"
#include "BlindMonsterCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BlindMonsterPatrol::UBTTask_BlindMonsterPatrol()
{
	NodeName = "Update Patrol Location";
}
// Use a predetermined route if there is one created and if not fallback to random waypoints inside the navmesh
EBTNodeResult::Type UBTTask_BlindMonsterPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
 
	if (!AIC || !BBComp) return EBTNodeResult::Failed;
 
	ABlindMonsterCharacter* Monster = Cast<ABlindMonsterCharacter>(AIC->GetPawn());
	if (!Monster) return EBTNodeResult::Failed;
	
	// finds the best route dependent on where the player is if a route is made, otherwise fallback to random
	Monster->SelectClosestRouteToPlayer();
	
	AActor* NextWaypoint = Monster->GetNextWaypoint();
	if (NextWaypoint)
	{
		BBComp->SetValueAsVector(GetSelectedBlackboardKey(), NextWaypoint->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}
 
	// fallback to random generated waypoints if there is no predetermined route set for the monster
	APawn* Pawn = AIC->GetPawn();
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
 
	if (!NavSys || !Pawn) return EBTNodeResult::Failed;
 
	FNavLocation NavLocation;
	if (NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), SearchRadius, NavLocation))
	{
		BBComp->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
		return EBTNodeResult::Succeeded;
	}
 
	return EBTNodeResult::Failed;
	
}
