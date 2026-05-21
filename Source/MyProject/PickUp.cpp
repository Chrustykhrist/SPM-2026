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
		if (ItemHit.GetActor()->Tags.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Item Hit Has No Tag"));
			return;
		}
		
		// Puts the item in the inventory and then removes it from the world
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();

		FName ItemName = ItemHit.GetActor()->Tags[0];

		if (ItemName == FName("PowerKey") && PS->GetCollectedItems()[FName("PowerKey")] >= 1)
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
			return;
		}
		
		if (PS->CollectedItems[ItemName] >= 3)
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
			return;
		}
		
		if (!PS->GetCollectedItems().Contains(ItemName))
		{
			UE_LOG(LogTemp, Error, TEXT("Item Not Found, Working on making it dynamic"));
			return;
			//PS->CollectedItems.Add(FName(ItemName), 1);
		} else
		{
			PS->CollectedItems[ItemName]++;
		}

		//UE_LOG(LogTemp, Display, TEXT("%s, %d"), *ItemHit.GetActor()->GetName(), PS->CollectedItems[ItemName]);
		
		ItemHit.GetActor()->Destroy();
	}

	if (bPushable)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *ButtonHit.GetComponent()->GetName());
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *ButtonHit.GetComponent()->ComponentTags[0].ToString());

		if (UKeyPadComponent* KP = Cast<UKeyPadComponent>(ButtonHit.GetActor()->GetComponentByClass(UKeyPadComponent::StaticClass())))
		{
			if (ButtonHit.GetComponent()->ComponentTags[0].IsEqual("Accept"))
			{
				// If the Accept button is press check if it is correct. Done im keypad
				KP->Accepted();
			}
			else if (ButtonHit.GetComponent()->ComponentTags[0].IsEqual("Clear"))
			{
				// If the clear button is called, clears the list of pressed buttons. Done in keypad
				KP->ClearPressed();
			}
			else
			{
				// if the amount pressed is 4 remove all the pressed buttons then add the pressed ones otherwise, just add the button
				if (KP->PressedButtons.Num() == 4)
				{
					KP->ClearPressed();
				}
				
				KP->Pressed(ButtonHit.GetComponent()->ComponentTags[0]);
				
				RecentlyPressed = ButtonHit.GetComponent()->ComponentTags[0];

			}
		} else if(ButtonHit.GetActor()->ActorHasTag("PowerSwitch"))
		{
			bPowerSwitchPushed = true;
		}
		
		if (AKeycardReader* KeycardReader = Cast<AKeycardReader>(ButtonHit.GetActor()))
		{
			/*APlayerController* PC = GetWorld()->GetFirstPlayerController();
			ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();
			KeycardReader->TryUnlock(PS);*/
			
			KeycardReader->OpenDoors();
		}
	}
}

bool UPickUp::GetPowerswitched()
{
	return bPowerSwitchPushed;
}

void UPickUp::RemoveNotif()
{
	if (IsValid(Notif))
	{
		Notif->RemoveFromParent();
		Notif = nullptr;
	}
}

