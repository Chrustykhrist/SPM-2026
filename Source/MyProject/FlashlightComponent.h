// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "FlashlightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UFlashlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlashlightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TurnOn();
	
	UFUNCTION()
	void TurnOff();
	
#pragma region GET/SET	
	UFUNCTION(BlueprintCallable)
	bool GetState() const { return bState; }
	
	UFUNCTION(BlueprintCallable)
	void SetState(bool NewState) { bState = NewState; }
	
	UFUNCTION(BlueprintCallable)
	float GetFlashlightDuration() const { return FlashlightDuration; }
	
	UFUNCTION(BlueprintCallable)
	void SetFlashlightDuration(float NewDuration) { FlashlightDuration = NewDuration; }
#pragma endregion
	
private:
	bool bState = false;
	
	UPROPERTY()
	USpotLightComponent* Flashlight;
	
	UPROPERTY()
	float FlashlightDuration = 5.0f;
};
