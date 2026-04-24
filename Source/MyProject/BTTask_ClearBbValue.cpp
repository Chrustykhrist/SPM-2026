// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBbValue.h"

#include "StalkerMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBbValue::UBTTask_ClearBbValue()
{
	NodeName = "Clear Blackboard Value";
}

EBTNodeResult::Type UBTTask_ClearBbValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	AStalkerMonsterAIController* AIC = Cast<AStalkerMonsterAIController>(OwnerComp.GetAIOwner());
	if (BBComp)
	{
		BBComp->ClearValue(GetSelectedBlackboardKey());
		if (AIC->bIsFleeing)
		{
			AIC->bIsFleeing = false;
			AIC->bHasTeleported = false;
		}
		return EBTNodeResult::Succeeded;
		
	}
	
	return EBTNodeResult::Failed;
	
}
