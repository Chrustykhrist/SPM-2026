// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"

#include "CustomPlayerState.h"
#include "KeyPadComponent.h"

// Sets default values for this component's properties
UPickUp::UPickUp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPickUp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPickUp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

/**
 * Checks if the player is able to pick up the item which the player is looking at and then puts the item in the inventory of the player if able
 */
void UPickUp::PickUp()
{
	// Vector to check where the player is looking and how far
	FVector PlayerPos = GetComponentLocation();
	FVector GrabVector = PlayerPos + GetForwardVector() * MaxGrabDistance;
	FVector PushVector = PlayerPos + GetForwardVector() * MaxPushDistance;

#if WITH_EDITOR
	// Shows where the player is looking
	DrawDebugLine(GetWorld(), PlayerPos, GrabVector, FColor::Red);
	DrawDebugSphere(GetWorld(), GrabVector, GrabRadius, 10, FColor::Blue);
#endif
	
	FHitResult ItemHit;
	FHitResult ButtonHit;

	// Shape that is used to check whether an item is hit
	FCollisionShape GrabVolume = FCollisionShape::MakeSphere(GrabRadius);

	// true if we hit an item that has the required hit channel as "Block", otherwise false
	bGrabbable = GetWorld()->SweepSingleByChannel(ItemHit, PlayerPos, GrabVector, FQuat::Identity, ECC_GameTraceChannel2, GrabVolume);
	bPushable = GetWorld()->LineTraceSingleByChannel(ButtonHit, PlayerPos, PushVector, ECC_GameTraceChannel3);
	
	if (bGrabbable)
	{
		// Puts the item in the inventory and then removes it
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();

		FName ItemName = ItemHit.GetActor()->Tags[0];

		PS->CollectedItems[ItemName]++;
		
		ItemHit.GetActor()->Destroy();
	}

	if (bPushable)
	{
		// Saves the pressed buttons and then checks if it is correct
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ButtonHit.GetComponent()->GetName());

		if (UKeyPadComponent* KP = Cast<UKeyPadComponent>(ButtonHit.GetActor()->GetComponentByClass(UKeyPadComponent::StaticClass())))
		{
			KP->Pressed(ButtonHit.GetComponent()->ComponentTags[0]);

			if (KP->PressedButtons.Num() == 4)
			{
				UE_LOG(LogTemp, Warning, TEXT("Succeded"));
			}
		}
	}
}

