// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "HidingComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UHidingComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UHidingComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void Hide();

	UFUNCTION(BlueprintCallable)
	void GetOut();

	UFUNCTION()
	AActor* GetPlayer() const;

	UPROPERTY(BlueprintReadWrite)
	bool bHiding = false;

private:
	UPROPERTY()
	AActor* Player;

	// Position to help leaving the locker
	FTransform EntryPosition;
};
