// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPadComponent.h"

#include "PickUp.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UKeyPadComponent::UKeyPadComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKeyPadComponent::BeginPlay()
{
	Super::BeginPlay();

	int num1 = FMath::RandRange(0, 9);
	int num2 = FMath::RandRange(0, 9);
	int num3 = FMath::RandRange(0, 9);
	int num4 = FMath::RandRange(0, 9);

	FString SNum1 = FString::FromInt(num1);
	FString SNum2 = FString::FromInt(num2);
	FString SNum3 = FString::FromInt(num3);
	FString SNum4 = FString::FromInt(num4);
	
	NeededCode.Add(FName(SNum1));
	NeededCode.Add(FName(SNum2));
	NeededCode.Add(FName(SNum3));
	NeededCode.Add(FName(SNum4));

	UE_LOG(LogTemp, Warning, TEXT("%s"), *SNum1);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *SNum2);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *SNum3);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *SNum4);

	// Find all the yaws of the doors
	for (int i = 0; i < Doors.Num(); i++)
	{
		DoorYaws.Add(Doors[i]->GetActorRotation().Yaw);
	}
	
	// ...
	
}


// Called every frame
void UKeyPadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// Turn the doors 90 degrees
	if (Turn)
	{
		for (int i = 0; i < Doors.Num(); i++)
		{
			if (Doors[i]->ActorHasTag("Left"))
			{
				Doors[i]->SetActorRotation(FMath::RInterpConstantTo(Doors[i]->GetActorRotation(), FRotator(0,  DoorYaws[i] + 90, 0), DeltaTime, 30));
			}
			else
			{
				Doors[i]->SetActorRotation(FMath::RInterpConstantTo(Doors[i]->GetActorRotation(), FRotator(0,  DoorYaws[i] - 90, 0), DeltaTime, 30));
			}
		}
	}
	
}

/**
 * Adds the number that is pressed to PressedButtons
 * 
 * @param number The pressed number, Tag of the component
 */
void UKeyPadComponent::Pressed(FName number)
{
	PressedButtons.Add(number);
}

/**
 * Opens the locked door when the correct numbers are inputted
 */
void UKeyPadComponent::Accepted()
{
	if (PressedButtons.Num() < 4)
	{
		return;
	}

	// Check if the code is correct
	for (int i = 0; i < NeededCode.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), (PressedButtons[i] != NeededCode[i]) ? TEXT("Different") : TEXT("Same"));
					
		if (PressedButtons[i] != NeededCode[i])
		{
			CorrectInput = false;
		}
	}

	// If correct, allow the door to turn, otherwise do nothing
	if (CorrectInput)
	{
		// TODO: Add unlock/open behaviour
		UE_LOG(LogTemp, Display, TEXT("Correct"));
		Turn = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Incorrect"));
		CorrectInput = true;
	}
}

/**
 * Removes all the buttons the user had pressed, called if more than 4 buttons are pressed or the clear button is pressed
 */
void UKeyPadComponent::ClearPressed()
{
	PressedButtons.Empty();
	UE_LOG(LogTemp, Display, TEXT("Clear Pressed"));
}

TArray<FName> UKeyPadComponent::GetNeededCode()
{
	return NeededCode;
}

