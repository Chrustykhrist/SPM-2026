// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerState.h"

#include <rapidjson/rapidjson.h>

#include "CustomGameInstance.h"
#include "GameFramework/GameModeBase.h"

ACustomPlayerState::ACustomPlayerState()
{
	// Make places for the items that will be added
	CollectedItems.Add(FName("Battery"), 0);
	CollectedItems.Add(FName("Medicine"), 0);
	CollectedItems.Add(FName("PowerKey"), 0);
	CollectedItems.Add(FName("KeycardA"), 0);
	CollectedItems.Add(FName("KeycardB"), 0);
	CollectedItems.Add(FName("Flashlight"), 0);
}

void ACustomPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Get and set the PlayerStart location for an initial spawn point
	FVector Start = GetWorld()->GetAuthGameMode()->FindPlayerStart(GetWorld()->GetFirstPlayerController())->GetActorLocation();
	
	SpawnTransform.SetLocation(Start);
	
	UCustomGameInstance* GI = Cast<UCustomGameInstance>(GetGameInstance());
	
	const TMap<FName, int>& Items = GI->GetInventory();
	CollectedItems.Append(Items);
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

/**
 * @return If we have the key or not
 */
bool ACustomPlayerState::CheckForPowerKey()
{
	if (CollectedItems[FName("PowerKey")] >= 1)
	{
		return true;
	}
	
	return false;
}

/**
 * @return If we have the required item or not, so this method could be reused for the other items as well
 */
bool ACustomPlayerState::HasRequiredItem(FName ItemName, int RequiredAmount)
{
	if (CollectedItems.Contains(ItemName))
	{
		return CollectedItems[ItemName] >= RequiredAmount;
	}
	
	return false;
}

void ACustomPlayerState::UseItem(FName ItemName, int AmountUsed)
{
	if (CollectedItems.Contains(ItemName))
	{
		CollectedItems[ItemName] -= AmountUsed;
	}
}

TMap<FName, int> ACustomPlayerState::GetCollectedItems()
{
	return CollectedItems;
}
