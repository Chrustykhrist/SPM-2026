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
	float MaxInteractionDistance = 120.0f;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionRadius = 10.0f;
	
	void BeginInteract();
	
	void InteractHeld(float Delta);
	
	void EndInteract();
	
	bool bIsInteracting = false;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TObjectPtr<AActor> CurrentInteractingActor;
	
	AActor* FindInteractingActor() const;
};
