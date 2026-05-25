// Fill out your copyright notice in the Description page of Project Settings.


#include "PushComponent.h"

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
	DrawDebugLine(GetWorld(), PlayerVector, PushDistance, FColor::Red, false, 2);
#endif
	
}

bool UPushComponent::GetPowerswitched()
{
	return false;
}

