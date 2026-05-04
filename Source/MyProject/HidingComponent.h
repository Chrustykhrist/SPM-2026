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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hiding")
	FVector HideOffset = FVector(0.0f, 0.0f, -40.0f); // Take character down when hiding -40 as default
	
	// Position to help leaving the locker
	FTransform EntryPosition;

private:
	UPROPERTY()
	AActor* Player;
};
