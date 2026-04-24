// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_PlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTS_PlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;
};
