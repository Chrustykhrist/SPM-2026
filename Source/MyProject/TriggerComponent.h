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
	/** Constructor */
	UTriggerComponent();
	
	/**	Begin Play */
	virtual void BeginPlay() override;
	
	/**	Tick */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * Position information about checkpoint
	 */
	
	// The position in the world
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnPosition;
	
	// The rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;
	
private:
	// The player
	UPROPERTY()
	AActor* Player;
	
	// Function to set the new spawn point
	UFUNCTION(BlueprintCallable)
	void SetSpawnPoint();
	
	//  Function to get the player if it enters the trigger
	UFUNCTION()
	AActor* GetPlayer() const;
	
	UPROPERTY()
	bool bNewSpawn = false;
};
