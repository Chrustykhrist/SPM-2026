// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQSContext_Player.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UEQSContext_Player : public UEnvQueryContext
{
	GENERATED_BODY()

public:	
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
