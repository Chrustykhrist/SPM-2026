// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameInstance.h"

#include "CustomPlayerState.h"
#include "Kismet/GameplayStatics.h"

UCustomGameInstance::UCustomGameInstance()
{
	
}

void UCustomGameInstance::Init()
{
	Super::Init();
}

void UCustomGameInstance::Shutdown()
{
	Super::Shutdown();
}


void UCustomGameInstance::OnSeamlessTravelDuringReplay()
{
	Super::OnSeamlessTravelDuringReplay();
	
	ACustomPlayerState* PS = Cast<ACustomPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	
	if (Inventory.IsEmpty())
	{
		const TMap<FName, int>& CopyMap = PS->GetCollectedItems();
		Inventory.Append(CopyMap);
	}
}

TMap<FName, int> UCustomGameInstance::GetInventory()
{
	return Inventory;
}

void UCustomGameInstance::SaveInventory()
{
	ACustomPlayerState* PS = Cast<ACustomPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	
	const TMap<FName, int>& CopyMap = PS->GetCollectedItems();
	Inventory.Append(CopyMap);
}