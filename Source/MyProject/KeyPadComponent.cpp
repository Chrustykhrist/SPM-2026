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

	// ...
	
}


// Called every frame
void UKeyPadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
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
	UE_LOG(LogTemp, Display, TEXT("Accepted"));
}

