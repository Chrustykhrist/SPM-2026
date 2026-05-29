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
	
	// Search radius for monster if it does not have a made route (using random points)
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float SearchRadius = 5000.0f;
};
