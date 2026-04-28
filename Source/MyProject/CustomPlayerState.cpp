// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerState.h"

#include "GameFramework/GameModeBase.h"

ACustomPlayerState::ACustomPlayerState()
{
	// Make places for the items that will be added
	CollectedItems.Add(FName("Battery"), 0);
	CollectedItems.Add(FName("Medicine"), 0);
}

void ACustomPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Get and set the PlayerStart location for an initial spawn point
	FVector Start = GetWorld()->GetAuthGameMode()->FindPlayerStart(GetWorld()->GetFirstPlayerController())->GetActorLocation();
	
	SpawnTransform.SetLocation(Start);
}

/**
 * Sets the new spawn position
 * 
 * @param NewSpawnTransform 
 */
void ACustomPlayerState::SetCheckPointTransform(FTransform NewSpawnTransform)
{
	SpawnTransform = NewSpawnTransform;
}

/**
 * @return The current spawn point
 */
FTransform ACustomPlayerState::GetCheckPointTransform() const
{
	return SpawnTransform;
}
