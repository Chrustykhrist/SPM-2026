// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CustomPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ACustomPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	FTransform SpawnTransform;
	
	UFUNCTION()
	void SetCheckPointTransform(FTransform NewSpawnTransform);
	
	UFUNCTION()
	FTransform GetCheckPointTransform() const;

	UPROPERTY()
	TMap<FName, int> CollectedItems;

	virtual void Tick(float DeltaTime) override;
};
