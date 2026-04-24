// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "CustomPlayerState.h"


UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Gets the player
	Player = GetPlayer();
	
	// Makes sure that a player was actually gotten and that we haven't already save the position
	if (Player != nullptr && !bNewSpawn)
	{
		// Makes this true so we don't save more than once
		bNewSpawn = true;
		// Calls the function to save the position
		SetSpawnPoint();
	}
}

/**
 * Saves the position of the checkpoint
 */
void UTriggerComponent::SetSpawnPoint()
{
	// Saves the player as a pawn
	APawn* PlayerPawn = Cast<APawn>(Player);
	
	// Checks to make sure if the player isn't nullptr
	if (PlayerPawn == nullptr)
	
	// Checks to make sure the player has a controller
	if (PlayerPawn->GetController())
	{
		// Gets the player state
		ACustomPlayerState* PS = PlayerPawn->GetPlayerState<ACustomPlayerState>();
		// Checks to make sure that the player state isn't nullptr
		check(PS != nullptr);
		
		// Sets the transform of the spawn point the transform of the object the code is on
		PS->SetCheckPointTransform(GetOwner()->GetActorTransform());
	}
}

/**
 * Returns the actor that has stepped into the trigger if they are the player
 * 
 * @return  The player
 */
AActor* UTriggerComponent::GetPlayer() const
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
			return Actor;
		}
	}
	// Otherwise return null
	return nullptr;
}
