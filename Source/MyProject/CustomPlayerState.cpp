// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerState.h"

void ACustomPlayerState::BeginPlay()
{
	Super::BeginPlay();

	CollectedItems.Add(FName("Battery"), 0);
	CollectedItems.Add(FName("Medicine"), 0);
}

void ACustomPlayerState::SetCheckPointTransform(FTransform NewSpawnTransform)
{
	SpawnTransform = NewSpawnTransform;
}

FTransform ACustomPlayerState::GetCheckPointTransform() const
{
	return SpawnTransform;
}

void ACustomPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

