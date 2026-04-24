// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PickUp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UPickUp : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickUp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Picks up the item that the player is looking at
	UFUNCTION()
	void PickUp();

private:
	// The max distance allowed between the players eyes and the item. Measured in cm
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 250;

	// The radius of the sphere that hits the item
	UPROPERTY(EditAnywhere)
	float GrabRadius = 20;

	// Bool to check if the item is grabbable
	bool bGrabbable = false;
};
