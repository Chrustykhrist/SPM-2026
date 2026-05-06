// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ValveComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValveCompleted);

class ATubeActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UValveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UValveComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Valve")
	float RequiredRotationDegrees = 360.0f;
	
	UPROPERTY(EditAnywhere, Category = "Valve")
	float InputSensitivity = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Valve")
	TObjectPtr<ATubeActor> LinkedTube;
	
	UPROPERTY(EditAnywhere, Category = "Valve")
	TObjectPtr<UStaticMeshComponent> ValveMesh;
	
	UPROPERTY(BlueprintAssignable)
	FOnValveCompleted OnValveCompleted;
	
	void BeginInteraction();
	
	void ApplyRotationDelta(float Delta);
	
	void EndInteraction();
	
	bool IsComplete() const {return bComplete;}
	
	bool IsBeingUsed() const {return bActive;}
	
	float GetProgress() const { return CurrentRotation / RequiredRotationDegrees; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float CurrentRotation = 0.0f;
	bool bActive = false;
	bool bComplete = false;
	
	void CompleteValve();
		
};
