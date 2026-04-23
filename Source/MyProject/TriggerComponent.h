// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UTriggerComponent();
	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	UPROPERTY()
	AActor* Player;
	
	UFUNCTION(BlueprintCallable)
	void SetSpawnPoint();
	
	UFUNCTION()
	AActor* GetPlayer();
	
	UPROPERTY()
	bool bNewSpawn = false;
};
