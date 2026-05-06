// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyPadComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UKeyPadComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKeyPadComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Adds the number that was pressed on the in-game keypad
	UFUNCTION()
	void Pressed(FName number);

	// Called when the correct numbers are inputed
	UFUNCTION()
	void Accepted();

	UFUNCTION()
	void ClearPressed();
	
	UPROPERTY()
	TArray<FName> PressedButtons;

	UPROPERTY()
	TArray<FName> NeededCode;

	UPROPERTY(EditAnywhere, Category="Doors")
	TArray<AActor*> Doors;

	bool CorrectInput = true;

	bool Turn = false;

private:
	UPROPERTY()
	TArray<float> DoorYaws;
};
