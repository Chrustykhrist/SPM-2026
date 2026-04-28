// Fill out your copyright notice in the Description page of Project Settings.


#include "StalkerMonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "StalkerMonsterCharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

// Create components as default for all the instances of this class
AStalkerMonsterAIController::AStalkerMonsterAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AStalkerMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
}

// Makes it so when this controller gets possesses the monster it executes the code inside here
void AStalkerMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// Initilizes the BB and runs BT when AIController possesses
	AStalkerMonsterCharacter* StalkerMonster = Cast<AStalkerMonsterCharacter>(InPawn);
	if (StalkerMonster && StalkerMonster->BehaviorTreeAsset)
	{
		BlackboardComp->InitializeBlackboard(*StalkerMonster->BehaviorTreeAsset->BlackboardAsset);
		RunBehaviorTree(StalkerMonster->BehaviorTreeAsset);
	}
}

// Flee method for stalker monster that updates bools and changes the value in the BB
void AStalkerMonsterAIController::TriggerFlee()
{
	bIsFleeing = true;
	bHasTeleported = false;
	BlackboardComp->SetValueAsBool("IsDetected", true);
	BlackboardComp->SetValueAsEnum("MonsterState", (uint8)EStalkerMonsterCharacterState::Fleeing);
}

// Stalk method that updates bools and changes the values in the BB
void AStalkerMonsterAIController::TriggerStalk()
{
	// Makes sure that the Fleeing behavior is done
	if (bIsFleeing) return;
	
	if (BlackboardComp->GetValueAsEnum("MonsterState") 
		== (uint8)EStalkerMonsterCharacterState::Stalking) return;
	
	bHasTeleported = false;
	BlackboardComp->SetValueAsEnum("MonsterState", (uint8)EStalkerMonsterCharacterState::Stalking);
	BlackboardComp->SetValueAsBool("IsDetected", false);
	UE_LOG(LogTemp, Warning, TEXT("TriggerStalk"));
	FindStalkLocation();
	
}

// This runs when the Stalk EQS runs in the BT to find location for monster to teleport to
void AStalkerMonsterAIController::FindStalkLocation()
{
	AStalkerMonsterCharacter* Monster = Cast<AStalkerMonsterCharacter>(GetPawn());
	if (!Monster || !StalkerQuery) return;
	
	FEnvQueryRequest Request(StalkerQuery, Monster);
	
	//Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &AStalkerMonsterAIController::OnStalkLocationFound);
	Request.Execute(EEnvQueryRunMode::SingleResult, this, &AStalkerMonsterAIController::OnStalkLocationFound);
}

// When location found for teleport, get the result and teleport if not already teleported
void AStalkerMonsterAIController::OnStalkLocationFound(TSharedPtr<FEnvQueryResult> Result)
{
	UE_LOG(LogTemp, Warning, TEXT("Location Found Before IsSuccessful %s"), Result->IsSuccessful() ? TEXT("true") : TEXT("false"));
	if (Result->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Location Found Successful"));
		
		FVector TargetLocation = Result->GetItemAsLocation(0);
		if (!bHasTeleported)
		{
			GetPawn()->SetActorLocation(TargetLocation);
			
			bHasTeleported = true;
			BlackboardComp->SetValueAsBool("HasTeleported", true);
			UE_LOG(LogTemp, Warning, TEXT("HasTeleported"));
			
		}
		
	}
}
