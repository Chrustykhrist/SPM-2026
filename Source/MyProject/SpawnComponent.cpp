// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnComponent.h"

#include "Commandlets/WorldPartitionCommandletHelpers.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"

// Sets default values for this component's properties
USpawnComponent::USpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USpawnComponent::SetNewSpawner()
{
	GetWorld()->GetAuthGameMode()->FindPlayerStart(GetWorld()->GetFirstPlayerController())->Destroy();
	
	APlayerStart* PlayerStart = CreateDefaultSubobject<APlayerStart>(TEXT("Player Start New"));
}


// Called every frame
void USpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

