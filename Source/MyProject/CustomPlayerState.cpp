// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerState.h"

void ACustomPlayerState::SetCheckPointTransform(FTransform NewSpawnTransform)
{
	SpawnTransform = NewSpawnTransform;
}

FTransform ACustomPlayerState::GetCheckPointTransform() const
{
	return SpawnTransform;
}

