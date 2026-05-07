// Fill out your copyright notice in the Description page of Project Settings.


#include "KeycardReader.h"

#include "CustomPlayerState.h"

// Sets default values
AKeycardReader::AKeycardReader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKeycardReader::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeycardReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/**
 *Tries to unlock the door by finding the playerstate and finding the unlock door funktion inside the blueprint
 *which requires Requiredkeycard, default keycard now is KeycardA (can change in instance editor)
 */
bool AKeycardReader::TryUnlock(ACustomPlayerState* PS)
{
	if (!PS) return false;
	UE_LOG(LogTemp, Warning, TEXT("PS found"));
	if (PS->HasRequiredItem(RequiredKeycard))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has required item"));
		if (LinkedDoor)
		{
			UE_LOG(LogTemp, Warning, TEXT("LinkedDoor"));
			// Finds the blueprint function that unlocks door, you guys spelled it wrong...
			UFunction* UnlockFunction = LinkedDoor->FindFunction(FName("UnlcokDoor"));
			if (UnlockFunction)
			{
				UE_LOG(LogTemp, Warning, TEXT("Door is being unlocked"));
				LinkedDoor->ProcessEvent(UnlockFunction, nullptr);
			}
		}
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("KeycardReader::TryUnlock() failed"));
	return false;
}