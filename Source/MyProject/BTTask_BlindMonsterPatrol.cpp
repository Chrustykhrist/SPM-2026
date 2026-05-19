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
	
	// AAIController* AIC = OwnerComp.GetAIOwner();
	// if (!AIC) return EBTNodeResult::Failed;
	//
	// ABlindMonsterCharacter* Monster = Cast<ABlindMonsterCharacter>(AIC->GetPawn());
	// UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	//
	// if (!Monster || !BBComp) return EBTNodeResult::Failed;
	//
	// // is the player in the alterroute, choose the correct route
	// bool bAlertRoute = BBComp->GetValueAsBool("IsAlerted");
	// AActor* NextWaypoint = Monster->GetNextWaypoint(bAlertRoute);
	//
	// if (NextWaypoint)
	// {
	// 	BBComp->SetValueAsVector(GetSelectedBlackboardKey(), NextWaypoint->GetActorLocation());
	// 	return EBTNodeResult::Succeeded;
	// }
	//
	// // Fallback to random patrol if there is no predetermined route
	// UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	// APawn* Pawn = AIC->GetPawn();
	// if (!NavSys || !Pawn) return EBTNodeResult::Failed;
	//
	// FNavLocation NavLocation;
	// if (NavSys->GetRandomReachablePointInRadius(
	// 		Pawn->GetActorLocation(), SearchRadius, NavLocation))
	// {
	// 	BBComp->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
	// 	return EBTNodeResult::Succeeded;
	// }
	//
	// return EBTNodeResult::Failed;
	
	// AAIController* AIController = OwnerComp.GetAIOwner();
	// if (!AIController) return EBTNodeResult::Failed;
	//
	// APawn* const Pawn = AIController->GetPawn();
	// UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	//
	// if (!NavSys || !Pawn) return EBTNodeResult::Failed;
	//
	// FVector NextLocation = Pawn->GetActorLocation();
	//
	// if (bUseRandomPatrol)
	// {
	// 	FNavLocation NavLocation;
	// 	if (NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), SearchRadius, NavLocation))
	// 	{
	// 		NextLocation = NavLocation.Location;
	// 	}
	// }
	// else if (SpecificPatrolPoints.Num() > 0)
	// {
	// 	int Index = FMath::RandRange(0, SpecificPatrolPoints.Num() - 1);
	// 	NextLocation = SpecificPatrolPoints[Index];
	// }
	//
	// OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NextLocation);
	//
	// return EBTNodeResult::Succeeded;
}
