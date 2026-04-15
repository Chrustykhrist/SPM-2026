// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BlindMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ABlindMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BTComp;
	
protected:
	virtual void OnPossess(class APawn* InPawn) override;
};
