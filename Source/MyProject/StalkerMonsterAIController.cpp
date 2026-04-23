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
	//FindStalkLocation();
}

// void AStalkerMonsterAIController::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 	
// }

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

void AStalkerMonsterAIController::TriggerFlee()
{
	bIsFleeing = true;
	bHasTeleported = false;
	BlackboardComp->SetValueAsBool("IsDetected", true);
	BlackboardComp->SetValueAsEnum("MonsterState", (uint8)EStalkerMonsterCharacterState::Fleeing);
}

void AStalkerMonsterAIController::TriggerStalk()
{
	if (BlackboardComp->GetValueAsEnum("MonsterState") == (uint8)EStalkerMonsterCharacterState::Stalking)
	{
		return;
	}
	
	if (bIsFleeing) return;
	
	BlackboardComp->SetValueAsEnum("MonsterState", (uint8)EStalkerMonsterCharacterState::Stalking);
	BlackboardComp->SetValueAsBool("IsDetected", false);
	UE_LOG(LogTemp, Warning, TEXT("TriggerStalk"));
	FindStalkLocation();
	
}

void AStalkerMonsterAIController::FindStalkLocation()
{
	AStalkerMonsterCharacter* Monster = Cast<AStalkerMonsterCharacter>(GetPawn());
	if (!Monster || !StalkerQuery) return;
	UE_LOG(LogTemp, Warning, TEXT("FindLocation"));
	FEnvQueryRequest Request(StalkerQuery, Monster);
	
	//Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &AStalkerMonsterAIController::OnStalkLocationFound);
	Request.Execute(EEnvQueryRunMode::SingleResult, this, &AStalkerMonsterAIController::OnStalkLocationFound);
}

void AStalkerMonsterAIController::OnStalkLocationFound(TSharedPtr<FEnvQueryResult> Result)
{
	UE_LOG(LogTemp, Warning, TEXT("Location Found Before IsSuccessful %s"), Result->IsSuccessful() ? TEXT("true") : TEXT("false"));
	if (Result->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Location Found Successful"));
		// AStalkerMonsterCharacter* Monster = Cast<AStalkerMonsterCharacter>(GetPawn());
		// if (!Monster) return;
		
		FVector TargetLocation = Result->GetItemAsLocation(0);
		//FRotator TargetRotation = Result->GetItemAsActor(0)->GetActorRotation();
		if (!bHasTeleported)
		{
			GetPawn()->SetActorLocation(TargetLocation);
			
			//Monster->TeleportTo(TargetLocation, TargetRotation);
			//TeleportTo(TargetLocation, TargetRotation,false, false);
			//SetActorLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics);
			bHasTeleported = true;
			BlackboardComp->SetValueAsBool("HasTeleported", true);
			UE_LOG(LogTemp, Warning, TEXT("HasTeleported"));
			
		}
		
		BlackboardComp->SetValueAsVector("TargetLocation", TargetLocation);
		UE_LOG(LogTemp, Warning, TEXT("LocationFound To Sneak"));
		DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 12, 
			FColor::Green, false, 8);
		
	}
}
