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
	// Variable for Behavior Tree component so you can select a BT in Unreal
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BTComp;
	
protected:
	// Override of OnPossess method to set the BTComp to a BT
	virtual void OnPossess(class APawn* InPawn) override;
};
