// Fill out your copyright notice in the Description page of Project Settings.


#include "BlindMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

void ABlindMonsterAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BTComp)
	{
		RunBehaviorTree(BTComp);
	}
}
