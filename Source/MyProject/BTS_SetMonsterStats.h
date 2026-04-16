// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SetMonsterStats.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTS_SetMonsterStats : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_SetMonsterStats();
	
protected:
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "MonsterStats", meta = (AllowPrivateAccess = true))
	float MonsterSpeed = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "MonsterStats", meta = (AllowPrivateAccess = true))
	float MonsterRotationSpeed = 360.0f;
};
