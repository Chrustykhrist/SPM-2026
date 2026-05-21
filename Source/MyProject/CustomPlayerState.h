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
	ACustomPlayerState();
	
	virtual void BeginPlay() override;

	// Set the new spawn point
	UFUNCTION()
	void SetCheckPointTransform(FTransform NewSpawnTransform);

	// Get spawn point
	UFUNCTION()
	FTransform GetCheckPointTransform() const;
	
	UFUNCTION(BlueprintCallable)
	bool CheckForPowerKey();
	
	UFUNCTION(BlueprintCallable)
	bool HasRequiredItem(FName ItemName, int RequiredAmount = 1);
	
	UFUNCTION(BlueprintCallable)
	void UseItem(FName ItemName, int AmountUsed = 1);
	
	UFUNCTION(BlueprintCallable)
	int GetItemCount(FName ItemName);
	
	UFUNCTION()
	TMap<FName, int> GetCollectedItems();
	
	// Inventory of the collected items, Key : Tag, Value : Number of items with that tag
	UPROPERTY()
	TMap<FName, int> CollectedItems;
	
	// Saved location to where the player should spawn after death
	UPROPERTY()
	FTransform SpawnTransform;
};
