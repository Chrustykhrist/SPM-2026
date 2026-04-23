// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "MovieSceneSequenceID.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"

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
	
	Player = GetPlayer();
	
	if (Player != nullptr && !bNewSpawn)
	{
		bNewSpawn = true;
		GetWorld()->GetAuthGameMode()->FindPlayerStart(GetWorld()->GetFirstPlayerController())->Destroy();
		SetSpawnPoint();
	}
}

void UTriggerComponent::SetSpawnPoint()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	
	FVector Pos = GetOwner()->GetActorLocation();
	FRotator Rot = GetOwner()->GetActorRotation();
	
	FActorSpawnParameters SpawnParams;
	
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AActor* NewStart = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), Pos, Rot, SpawnParams);
}

AActor* UTriggerComponent::GetPlayer()
{
	TArray<AActor*> PlayerActors;
	GetOverlappingActors(PlayerActors);
	
	for (AActor* Actor : PlayerActors)
	{
		if (Actor->ActorHasTag("Player"))
		{
			return Actor;
		}
	}
	
	return nullptr;
}
