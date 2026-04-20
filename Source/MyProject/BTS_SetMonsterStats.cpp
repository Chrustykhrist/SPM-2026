// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_SetMonsterStats.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTS_SetMonsterStats::UBTS_SetMonsterStats()
{
	NodeName = "Set Monster Stats";
	
	// Need to set to true because false as standard otherwise OnBecomeRelevant doest get called
	bNotifyBecomeRelevant = true;
}

void UBTS_SetMonsterStats::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	UE_LOG(LogTemp, Warning, TEXT("Monster Stats Relevant"));
	AAIController* MonsterController = OwnerComp.GetAIOwner();
	if (MonsterController)
	{
		
		ACharacter* Monster = Cast<ACharacter>(MonsterController->GetPawn());
		
		if (Monster)
		{
			
			UCharacterMovementComponent* MonsterMovement = Monster->GetCharacterMovement();
			
			if (MonsterMovement)
			{
				MonsterMovement->MaxWalkSpeed = MonsterSpeed;
				MonsterMovement->RotationRate.Yaw = MonsterRotationSpeed;
				UE_LOG(LogTemp, Warning, TEXT("Speed: %f and Rotation: %s"), MonsterMovement->MaxWalkSpeed, *MonsterMovement->RotationRate.ToString());
			}
			
		}
		
	}
	
}
