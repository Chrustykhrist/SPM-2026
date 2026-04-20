// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "StalkerMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AStalkerMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	// Constructor
	AStalkerMonsterAIController();
	
	void FindStalkLocation();
	
	void OnStalkLocationFound(TSharedPtr<FEnvQueryResult> Result);
	
protected:
	virtual void BeginPlay() override;
	
	// Override of OnPossess to set the behaviortree on possess
	virtual void OnPossess(APawn* InPawn) override;
	
	//BhaviorTree component for the monster
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTreeComponent* BehaviorTreeComp;
	
	//BlackboardComponent for the monster
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardComponent* BlackboardComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetPlayerKey = "TargetPlayer";
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName MonsterStateKey = "MonsterState";
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UEnvQuery* StalkerQuery;
	
};
