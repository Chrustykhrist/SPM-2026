// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

class ASpotLight;

UCLASS()
class MYPROJECT_API AGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void TurnOn();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GeneratorMesh;
	
	UPROPERTY(EditAnywhere, Category="Changes on Turn on")
	TArray<ASpotLight*> SpotLights;
	
	UPROPERTY(EditAnywhere, Category="Changes on Turn on")
	TArray<AActor*> Openings;
	
	UPROPERTY()
	TArray<float> DoorOffsets;
	
	UPROPERTY()
	TArray<float> DoorYaws;
	
	UPROPERTY(EditAnywhere, Category="Changes to")
	float IntensityIncrease = 5;
	
	UPROPERTY(EditAnywhere, Category="Changes to")
	float AttenuationIncrease = 500;
	
	UPROPERTY(EditAnywhere, Category="Changes to")
	FVector MoveDistance = FVector(300, 0, 0);
	
	bool bOpen = false;
	
	UPROPERTY()
	TArray<FVector> StartLocations;
};
