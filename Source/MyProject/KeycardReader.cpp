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
	
	for (int i = 0; i < LinkedDoor.Num(); i++)
	{
		DoorYaws.Add(LinkedDoor[i]->GetActorRotation().Yaw);
		DoorOffsets.Add(0.0f);
	}
	
}

// Called every frame
void AKeycardReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (bOpenDoors)
	{
		for (int i = 0; i < LinkedDoor.Num(); i++)
		{
			FRotator TargetRotation = FRotator(0, DoorYaws[i] + DoorOffsets[i], 0);
			LinkedDoor[i]->SetActorRotation(FMath::RInterpConstantTo(LinkedDoor[i]->GetActorRotation(), TargetRotation, DeltaTime, 30));
		}
	}
	
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
		if (!LinkedDoor.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("LinkedDoor"));
			// Finds the blueprint function that unlocks door, you guys spelled it wrong...
			// Finnds all the doors that is connected to the reader
			UFunction* UnlockFunction = nullptr;
			for (AActor* CurrentDoor : LinkedDoor)
			{
				UnlockFunction = CurrentDoor->FindFunction(FName("UnlcokDoor"));
				if (UnlockFunction)
				{
					UE_LOG(LogTemp, Warning, TEXT("Door is being unlocked"));
					CurrentDoor->ProcessEvent(UnlockFunction, nullptr);
				}
			}
			
		}
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("KeycardReader::TryUnlock() failed"));
	return false;
}

void AKeycardReader::OpenDoors()
{
	ACustomPlayerState* PS = Cast<ACustomPlayerState>(GetWorld()->GetFirstPlayerController()->GetPlayerState<ACustomPlayerState>());
	
	if (!PS->HasRequiredItem(RequiredKeycard)) return;
	
	PS->CollectedItems[RequiredKeycard]--;
	
	APawn* Pawn = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (Pawn == nullptr) return;
	
	FVector PawnLocation = Pawn->GetActorLocation();
	
	for (int i = 0; i < LinkedDoor.Num(); i++)
	{
		FVector DoorForward = FRotator(0, DoorYaws[i], 0).Vector();
		FVector ToDoor = (LinkedDoor[i]->GetActorLocation() - PawnLocation).GetSafeNormal();
		
		float Dot = FVector::DotProduct(DoorForward, ToDoor);
		DoorOffsets[i] = (Dot >= 0.0f) ? 90.0f : -90.0f;
	}
	
	bOpenDoors = true;
}
