// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_InvestigateArea.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_InvestigateArea::UBTTask_InvestigateArea()
{
	NodeName = "Investigate Area";
}

EBTNodeResult::Type UBTTask_InvestigateArea::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	UBlackboardComponent* const BBComp = OwnerComp.GetBlackboardComponent();
	
	if (!NavSys || !BBComp) return EBTNodeResult::Failed;
	
	FVector const CenterLocation = BBComp->GetValueAsVector(GetSelectedBlackboardKey());
	FNavLocation NextLocation;
	
	if (NavSys->GetRandomReachablePointInRadius(CenterLocation, InvestigateRadius, NextLocation))
	{
		BBComp->SetValueAsVector(GetSelectedBlackboardKey(), NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Kunde inte hitta någon punkt"))
	return EBTNodeResult::Succeeded;
	
}