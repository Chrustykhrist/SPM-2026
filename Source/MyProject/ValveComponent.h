// Fill out your copyright notice in the Description page of Project Settings.


#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ValveComponent.generated.h"


class ACustomPlayerState;
class UHighlightInteractablesComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValveCompleted);


class ATubeActor;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UValveComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	UValveComponent();
  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Valve")
	float RequiredRotationDegrees = 720.0f;
  
	UPROPERTY(EditAnywhere, Category = "Valve")
	float InputSensitivity = 1.0f;
  
	UPROPERTY(EditAnywhere, Category = "Valve")
	TObjectPtr<ATubeActor> LinkedTube;
  
	UPROPERTY(EditAnywhere, Category = "Valve")
	TObjectPtr<UStaticMeshComponent> ValveMesh;
  
	UPROPERTY(BlueprintAssignable)
	FOnValveCompleted OnValveCompleted;
  
	void BeginInteraction();
  
	void ApplyRotationDelta(float Delta);
  
	void EndInteraction();
	
	UFUNCTION(BlueprintCallable, Category = "Valve")
	bool IsComplete() const {return bComplete;}
  
	bool IsBeingUsed() const {return bActive;}
  
	float GetProgress() const { return CurrentRotation / RequiredRotationDegrees; }
	
	void RestoreState(ACustomPlayerState* PS);
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
	
	FRotator InitialMeshRotation = FRotator::ZeroRotator;
	
	void CompleteValve();
	
	void FindHighlightAndDestroy();
	
	UHighlightInteractablesComponent* HighlightedComp = nullptr;
};
