// Fill out your copyright notice in the Description page of Project Settings.


#include "StalkerMonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "StalkerMonsterCharacter.h"

AStalkerMonsterAIController::AStalkerMonsterAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AStalkerMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	AStalkerMonsterCharacter* StalkerMonster = Cast<AStalkerMonsterCharacter>(InPawn);
	if (StalkerMonster && StalkerMonster->BehaviorTreeAsset)
	{
		BlackboardComp->InitializeBlackboard(*StalkerMonster->BehaviorTreeAsset->BlackboardAsset);
		BehaviorTreeComp->StartTree(*StalkerMonster->BehaviorTreeAsset);
	}
}
