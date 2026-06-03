// Fill out your copyright notice in the Description page of Project Settings.


#include "PushComponent.h"

#include "CustomPlayerState.h"
#include "KeycardReader.h"
#include "KeyPadComponent.h"

// Sets default values for this component's properties
UPushComponent::UPushComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPushComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPushComponent::Push()
{
	FVector PlayerVector = GetComponentLocation();
	FVector PushDistance = PlayerVector + GetForwardVector() * MaxPushDistance;
	
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), PlayerVector, PushDistance, FColor::Red, false, 0.5);
#endif
	
	FHitResult HitResult;
	
	bPushable = GetWorld()->LineTraceSingleByChannel(HitResult, PlayerVector, PushDistance, ECC_GameTraceChannel3);
	
	if (bPushable)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();
		
		if (UKeyPadComponent* KP = Cast<UKeyPadComponent>(HitResult.GetActor()->GetComponentByClass(UKeyPadComponent::StaticClass())))
		{
			HandleKeyPad(HitResult, KP);
		}
		else if (HitResult.GetActor()->ActorHasTag("PowerSwitch"))
		{
			bPowerSwitchPushed = true;
		}
		else if (AKeycardReader* KR = Cast<AKeycardReader>(HitResult.GetActor()))
		{
			KR->OpenDoors();
		}
	}
}

bool UPushComponent::GetPowerSwitched()
{
	return bPowerSwitchPushed;
}

void UPushComponent::HandleKeyPad(const FHitResult& HitResult, UKeyPadComponent* KP)
{
	if (HitResult.GetComponent()->ComponentTags[0].IsEqual("Accept"))
	{
		KP->Accepted();
	}
	else if (HitResult.GetComponent()->ComponentTags[0].IsEqual("Clear"))
	{
		KP->ClearPressed();
	}
	else
	{
		if (KP->PressedButtons.Num() == 4)
		{
			KP->ClearPressed();
		}
				
		KP->Pressed(HitResult.GetComponent()->ComponentTags[0]);
	}
}

