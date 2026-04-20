// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StalkerMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AStalkerMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	AStalkerMonsterAIController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTreeComponent* BehaviorTreeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardComponent* BlackboardComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetPlayerKey = "TargetPlayer";
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName MonsterStateKey = "MonsterState";
	
};
