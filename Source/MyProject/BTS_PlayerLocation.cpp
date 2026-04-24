// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void UBTS_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerActor)
	{
		//FVector PlayerLocation = PlayerActor->GetActorLocation();
		
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerActor->GetActorLocation());
	}
}
