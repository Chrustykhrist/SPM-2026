// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"

#include "CustomPlayerState.h"
#include "KeycardReader.h"
#include "KeyPadComponent.h"
#include "Blueprint/UserWidget.h"

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
 * Checks if the player is able to press or pick up the object in front of them
 */
void UPickUp::PickUp()
{
	// Vector to check where the player is looking and how far
	FVector PlayerPos = GetComponentLocation();
	FVector GrabVector = PlayerPos + GetForwardVector() * MaxGrabDistance;

#if WITH_EDITOR
	// Shows where the player is looking
	DrawDebugSphere(GetWorld(), GrabVector, GrabRadius, 16, FColor::Blue, false, 0.5);
#endif
	
	FHitResult ItemHit;

	// Shape that is used to check whether an item is hit
	FCollisionShape GrabVolume = FCollisionShape::MakeSphere(GrabRadius);

	// true if we hit an item that has the required hit channel as "Block", otherwise false
	bGrabbable = GetWorld()->SweepSingleByChannel(ItemHit, PlayerPos, GrabVector, FQuat::Identity, ECC_GameTraceChannel2, GrabVolume);
	
	ACustomPlayerState* PS = GetWorld()->GetFirstPlayerController()->GetPlayerState<ACustomPlayerState>();
	
	if (bGrabbable)
	{
		if (ItemHit.GetActor()->Tags.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Item Hit Has No Tag"));
			return;
		}
		
		// Puts the item in the inventory and then removes it from the world
		FName ItemName = ItemHit.GetActor()->Tags[0];

		if (ItemName == FName("PowerKey") && PS->GetCollectedItems()[FName("PowerKey")] >= 1)
		{
			ShowNotif();
			return;
		}
		
		if (ItemHit.GetActor()->Tags.Num() >= 2)
		{
			if (ItemHit.GetActor()->Tags[1] == FName("Keycard"))
			{
				
			}
		}
		
		if (PS->CollectedItems[ItemName] >= 3)
		{
			ShowNotif();
			return;
		}
		
		if (!PS->GetCollectedItems().Contains(ItemName))
		{
			UE_LOG(LogTemp, Error, TEXT("Item Not Found, Working on making it dynamic"));
			return;
		}

		CollectItem(ItemHit, PS);
	}
}

void UPickUp::RemoveNotif()
{
	if (IsValid(Notif))
	{
		Notif->RemoveFromParent();
		Notif = nullptr;
	}
}

void UPickUp::CollectItem(const FHitResult& Hit, ACustomPlayerState* PS)
{
	PS->CollectedItems[Hit.GetActor()->Tags[0]]++;
	
	Hit.GetActor()->Destroy();
}

void UPickUp::ShowNotif()
{
	if (IsValid(NotifClass) && !IsValid(Notif))
	{
		Notif = CreateWidget(GetWorld(), NotifClass);
	}
	if (IsValid(Notif))
	{
		Notif->AddToViewport();
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPickUp::RemoveNotif, RestartDelay, false);
}

