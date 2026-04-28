// Fill out your copyright notice in the Description page of Project Settings.


#include "HidingComponent.h"

#include "PlayerCharacter.h"

UHidingComponent::UHidingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHidingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHidingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Player = GetPlayer();
}

/**
 * Moves the player inside the locker to hide them
 */
void UHidingComponent::Hide()
{

	if (bHiding == true)
	{
		return;
	}
	
	APawn* PP = Cast<APawn>(Player);

	if (PP == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player pawn is null"));
		return;
	}

	EntryPosition = PP->GetActorTransform();

	PP->SetActorTransform(GetOwner()->GetActorTransform());

	bHiding = true;
}

/**
 * Puts the player back in the position they entered at
 */
void UHidingComponent::GetOut()
{
	APawn* PlayerPawn = Cast<APawn>(Player);

	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player pawn is null"));
		return;
	}

	PlayerPawn->SetActorTransform(EntryPosition);
	
	bHiding = false;
}

/**
 * makes sure that it is the player that has entered the box trigger
 * @return The player
 */
AActor* UHidingComponent::GetPlayer() const
{
	// An array that will contain all the actors that are in the collider
	TArray<AActor*> PlayerActors;
	// Adds the actor that is in the collider to the array
	GetOverlappingActors(PlayerActors);
	
	// Goes through all the actors that are in the array
	for (AActor* Actor : PlayerActors)
	{
		// If the actor has the tag "PLayer" which only the player should have, returns that actor
		if (Actor->ActorHasTag("Player"))
		{
			APlayerCharacter* PC = Cast<APlayerCharacter>(Actor);

			if (PC != nullptr)
			{
				PC->SetHidingComponent(Cast<UHidingComponent>(GetOwner()->GetComponentByClass(UHidingComponent::StaticClass())));
			} 
			
			return Actor;
		}
	}
	// Otherwise return null
	return nullptr;
}
