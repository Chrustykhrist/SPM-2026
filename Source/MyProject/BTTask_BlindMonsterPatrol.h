// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BlindMonsterPatrol.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTTask_BlindMonsterPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_BlindMonsterPatrol();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Patrol")
	bool bUseRandomPatrol = true;
	
	UPROPERTY(EditAnywhere, Category = "Patrol", meta = (EditCondition = "!bUseRandomPatrol"))
	TArray<FVector> SpecificPatrolPoints;
	
	UPROPERTY(EditAnywhere, Category = "Patrol", meta = (EditCondition = "bUseRandomPatrol"))
	float SearchRadius = 5000.0f;
};
