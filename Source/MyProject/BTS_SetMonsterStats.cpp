// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_SetMonsterStats.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FootstepComponent.h"

UBTS_SetMonsterStats::UBTS_SetMonsterStats()
{
	NodeName = "Set Monster Stats";
	
	// Need to set to true because false as standard otherwise OnBecomeRelevant doest get called
	bNotifyBecomeRelevant = true;
}

void UBTS_SetMonsterStats::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	AAIController* MonsterController = OwnerComp.GetAIOwner();
	if (MonsterController)
	{
		
		ACharacter* Monster = Cast<ACharacter>(MonsterController->GetPawn());
		
		if (Monster)
		{
			UFootstepComponent* MonsterFootstepComp = Monster->FindComponentByClass<UFootstepComponent>();
			UCharacterMovementComponent* MonsterMovement = Monster->GetCharacterMovement();
			
			if (MonsterMovement)
			{
				MonsterMovement->MaxWalkSpeed = MonsterSpeed;
				MonsterMovement->RotationRate.Yaw = MonsterRotationSpeed;
				
				if (MonsterFootstepComp)
				{
					if (MonsterSpeed <= 250.0f)
					{
						MonsterFootstepComp->SetMovementState(EMovementState::Sneaking); 
					}
					else if (MonsterSpeed <= 500.0f)
					{
						MonsterFootstepComp->SetMovementState(EMovementState::Walking); 
					}
					else
					{
						MonsterFootstepComp->SetMovementState(EMovementState::Sprinting); 
					}
					
				}
				
				//UE_LOG(LogTemp, Warning, TEXT("Speed: %f and Rotation: %s"), MonsterMovement->MaxWalkSpeed, *MonsterMovement->RotationRate.ToString());
			}
			
		}
		
	}
	
}
