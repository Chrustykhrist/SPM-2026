// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TubeActor.generated.h"

UCLASS()
class MYPROJECT_API ATubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATubeActor();
	
	UPROPERTY(VisibleAnywhere, Category = "Tube")
	TObjectPtr<UStaticMeshComponent> FluidMesh;
	
	UPROPERTY(EditAnywhere, Category = "Tube")
	FName FillLevelParameterName  = TEXT("FillLevel");
	
	UPROPERTY(EditAnywhere, Category = "Tube")
	float DrainDuration = 8.0f;
	
	UFUNCTION(BlueprintCallable)
	void Drain();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	bool bDraining = false;
	float CurrentFillLevel = 1.0f;
	TObjectPtr<UMaterialInstanceDynamic> DynamicFluidMaterial;
	FVector UnderTheMap;
	FVector StartLocation;
};
