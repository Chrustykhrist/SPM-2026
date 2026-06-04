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

	UFUNCTION(BlueprintCallable)
	void TurnOn();
	
	UFUNCTION(BlueprintCallable)
	void TurnOff();
	
#pragma region GET/SET	
	UFUNCTION(BlueprintCallable)
	bool GetState() const { return bState; }
	
	UFUNCTION(BlueprintCallable)
	void SetState(bool NewState) { bState = NewState; }
	
	UFUNCTION(BlueprintCallable)
	float GetFlashlightDuration() const { return FlashlightDuration; }
	
	UFUNCTION(BlueprintCallable)
	float GetMaxDuration() const { return MaxFlashlightDuration; }
	
	UFUNCTION(BlueprintCallable)
	void SetFlashlightDuration(float NewDuration) { FlashlightDuration = NewDuration; }
	
	UFUNCTION(BlueprintCallable)
	void SetFlashlightColor(FLinearColor NewColor) { Flashlight->SetLightColor(NewColor); }
#pragma endregion
	
	UFUNCTION()
	void Recharge();
	
	UFUNCTION()
	void Deteriorate() const;
	
private:
	bool bState = false;
	
	UPROPERTY()
	USpotLightComponent* Flashlight;
	
	UPROPERTY()
	float FlashlightDuration;
	
	UPROPERTY(EditAnywhere)
	float MaxFlashlightDuration = 10.0f;
	
	UPROPERTY(EditAnywhere)
	float MaxAttenuation = 1500.0f;
	
	UPROPERTY(EditAnywhere)
	float MaxIntensity = 15.0f;
	
	UPROPERTY(EditAnywhere)
	float DeterioratedAttenuation = 500.0f;
	
	UPROPERTY(EditAnywhere)
	float DeterioratedIntensity = 2.0f;
};
