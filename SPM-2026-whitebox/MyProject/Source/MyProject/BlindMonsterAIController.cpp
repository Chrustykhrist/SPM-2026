// Fill out your copyright notice in the Description page of Project Settings.


#include "BlindMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"


// The override of OnPossess that calls Super of method and adds an extra part that checks if you 
// put the BT inside the BindMonsterAIController inside unreal
void ABlindMonsterAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BTComp)
	{
		RunBehaviorTree(BTComp);
	}
}
