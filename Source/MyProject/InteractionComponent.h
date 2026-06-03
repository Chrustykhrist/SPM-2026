// Fill out your copyright notice in the Description page of Project Settings.


#pragma once


#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()
  
public:
	// Sets default values for this component's properties
	UInteractionComponent();
  
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float MaxInteractionDistance = 200.0f;
  
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionRadius = 50.0f;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float ValveRotationSpeed = 90.0f;
	
	void BeginInteract();
	
	void InteractHeld();
	
	void EndInteract();
  
	bool bIsInteracting = false;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// UPROPERTY(EditAnywhere, Category = "Interaction")
	// float IgnoreCentreThreshold = 100.0f;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	TObjectPtr<AActor> CurrentInteractingActor;
  
	AActor* FindInteractingActor() const;
  
	float AcceptableLookRatio = 0.7f;
  
	// FVector2D LastMousePos = FVector2D::ZeroVector;
	// bool bHasLastMousePos = false;
	//
	// FVector2D VirtualStickPos = FVector2D::ZeroVector;
	// bool bHasLastStickAngle = false;
	// float LastStickAngle = 0.0f;
	float RotationDelta = 0.0f;
	// float ComputeCircularDelta();
	// float ComputeCircularDeltaFromStick(FVector2D StickInput);
};
