// Fill out your copyright notice in the Description page of Project Settings.


#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyPadComponent.generated.h"


class UFMODEvent;
class ACustomPlayerState;


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
	
	// Remove all the pressed buttons form the array of pressed buttons.
	UFUNCTION()
	void ClearPressed();
	
	UFUNCTION()
	void OpenDoors();
  
	UFUNCTION(BlueprintCallable, Blueprintable)
	TArray<FName> GetNeededCode();
  
	UFUNCTION(BlueprintCallable, Blueprintable)
	FString ShowPressed();
	
	// What buttons have been pressed
	UPROPERTY()
	TArray<FName> PressedButtons;
	
	// The code that needs to be inputted
	UPROPERTY(meta=(BlueprintCallable))
	TArray<FName> NeededCode;
	
	// The list of doors that are to open
	UPROPERTY(EditAnywhere, Category="Doors", BlueprintReadOnly)
	TArray<AActor*> Doors;
	
	// True if correct buttons in correct sequence, otherwise false
	bool bCorrectInput = true;
	
	// True if doors should turn
	UPROPERTY( BlueprintReadOnly )
	bool bTurn = false;
  
	void RestoreState(ACustomPlayerState* PS);
	
	UFUNCTION(BlueprintCallable)
	void SetRecentlyPressed(FName number) { RecentlyPressed = number; }
	
	UFUNCTION(BlueprintCallable)
	FName GetRecentlyPressed() { return RecentlyPressed; }

private:
	// Array of all the doors yaws
	UPROPERTY()
	TArray<float> DoorYaws;
	
	UPROPERTY()
	TArray<float> DoorOffsets;
	
	UPROPERTY()
	FName RecentlyPressed;
	
	UPROPERTY(EditAnywhere)
	UFMODEvent* PressEvent;
};
