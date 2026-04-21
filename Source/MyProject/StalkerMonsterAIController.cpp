// Fill out your copyright notice in the Description page of Project Settings.


#include "StalkerMonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "StalkerMonsterCharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

AStalkerMonsterAIController::AStalkerMonsterAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AStalkerMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
	FindStalkLocation();
}

void AStalkerMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	AStalkerMonsterCharacter* StalkerMonster = Cast<AStalkerMonsterCharacter>(InPawn);
	if (StalkerMonster && StalkerMonster->BehaviorTreeAsset)
	{
		BlackboardComp->InitializeBlackboard(*StalkerMonster->BehaviorTreeAsset->BlackboardAsset);
		//BehaviorTreeComp->StartTree(*StalkerMonster->BehaviorTreeAsset);
		//BehaviorTreeComp->RunBehaviorTree(*StalkerMonster->BehaviorTreeAsset);
		RunBehaviorTree(StalkerMonster->BehaviorTreeAsset);
	}
}

void AStalkerMonsterAIController::FindStalkLocation()
{
	AStalkerMonsterCharacter* Monster = Cast<AStalkerMonsterCharacter>(GetPawn());
	if (!Monster || !StalkerQuery) return;
	
	FEnvQueryRequest Request(StalkerQuery, Monster);
	
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &AStalkerMonsterAIController::OnStalkLocationFound);
}

void AStalkerMonsterAIController::OnStalkLocationFound(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsSuccessful())
	{
		FVector TargetLocation = Result->GetItemAsLocation(0);
		
		BlackboardComp->SetValueAsVector("TargetLocation", TargetLocation);
		UE_LOG(LogTemp, Warning, TEXT("LocationFound"));
		DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 12, 
			FColor::Green, false, 8);
	}
}
