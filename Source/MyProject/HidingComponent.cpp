// Fill out your copyright notice in the Description page of Project Settings.


#include "HidingComponent.h"

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

	if (Player)
	{
		Hide();
	}
}

void UHidingComponent::Hide()
{
	APawn* PP = Cast<APawn>(Player);

	if (PP == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player pawn is null"));
		return;
	}

	EntryPosition = PP->GetActorTransform();

	PP->SetActorTransform(GetOwner()->GetActorTransform());
}

AActor* UHidingComponent::GetPlayer()
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
