// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "ValveActor.generated.h"

class UValveComponent;

UCLASS()
class MYPROJECT_API AValveActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AValveActor();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ValveBaseMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ValveWheelMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UValveComponent> ValveComponent;
	
	virtual void OnInteractBegin(APlayerController* PC) override;
	
	virtual void OnInteractHold(APlayerController* PC, float Delta) override;
	
	virtual void OnInteractEnd(APlayerController* PC) override;
	
	virtual bool CanInteract() const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
