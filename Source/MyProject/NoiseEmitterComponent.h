// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NoiseEmitterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UNoiseEmitterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNoiseEmitterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Noise")
	class USoundBase* Sound;
	
	UPROPERTY(EditAnywhere, Category = "Noise")
	float DefaultLoudness = 1.0f;
	
	UFUNCTION(BlueprintCallable, Category = "Noise", meta = (AllowPrivateAccess = true))
	void TriggerNoise(APawn* Instigator, float LoudnessMultiplier = 1.0f, FVector Location = FVector::ZeroVector);
		
};
