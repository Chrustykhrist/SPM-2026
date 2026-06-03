// Fill out your copyright notice in the Description page of Project Settings.


#include "HidingComponent.h"

#include "HorrorPlayerController.h"
#include "PlayerCharacter.h"
#include "Camera/CameraActor.h"
#include "GameFramework/CharacterMovementComponent.h"

UHidingComponent::UHidingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UHidingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// LockerDoor = Cast<UStaticMeshComponent>(GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Door"))[0]);
	//
	// if (LockerDoor)
	// {
	// 	DoorRotation = LockerDoor->GetRelativeRotation();
	// }
}

void UHidingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bHiding)
	{
		Player = GetPlayer();
	}
	
	// if (bOpen)
	// {
	// 	FRotator NewRotation = DoorRotation + FRotator(0, 140, 0);
	// 	
	// 	LockerDoor->SetRelativeRotation(FMath::RInterpConstantTo(DoorRotation, NewRotation, DeltaTime, 10));
	// }
	
}

/**
 * Moves the player inside the locker to hide them
 */
void UHidingComponent::Hide()
{
	if (bHiding == true)
	{
		return;
	}
	
	// bOpen = true;
	
	ACharacter* PP = Cast<ACharacter>(Player);
	
	if (PP == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player pawn is null"));
		return;
	}
	
	EntryPosition = PP->GetActorTransform();
	
	FVector FinalOffset = HideOffset;
	
	// Calculate the position
	FVector NewLocation = GetOwner()->GetActorLocation() + HideOffset;
    
	// Set rotation to the same as the locker
	FRotator NewRotation = GetOwner()->GetActorRotation();
	
	// Stop player from moving
	if (UCharacterMovementComponent* CMC = Cast<UCharacterMovementComponent>(PP->GetMovementComponent()))
	{
		CMC->StopMovementImmediately();
		CMC->GravityScale = 0.0f;
		CMC->SetMovementMode(MOVE_None);
	}
	
	// Makes player invisible
	PP->SetActorEnableCollision(false);
    
	// Teleport player with offset
	PP->SetActorLocationAndRotation(NewLocation, NewRotation);
	
	// Rotate player to look out of locker
	if (AHorrorPlayerController* PC = Cast<AHorrorPlayerController>(PP->GetController()))
	{
		PC->SetControlRotation(NewRotation);
	}
	
	bHiding = true;
	
	//CloseDoor();
}

/**
 * Puts the player back in the position they entered at
 */
void UHidingComponent::GetOut()
{
	APawn* PlayerPawn = Cast<APawn>(Player);

	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player pawn is null"));
		return;
	}
	
	
	ACharacter* PP = Cast<ACharacter>(Player);
	
	// Allow player move
	if (UCharacterMovementComponent* CMC = Cast<UCharacterMovementComponent>(PP->GetMovementComponent()))
	{
		CMC->GravityScale = 1.0f;
		CMC->SetMovementMode(MOVE_Walking);
	}

	PlayerPawn->SetActorTransform(EntryPosition);
	
	PlayerPawn->GetController()->SetControlRotation(EntryPosition.GetRotation().Rotator());
	
	PlayerPawn->SetActorEnableCollision(true);
	
	bHiding = false;
	
}

/**
 * makes sure that it is the player that has entered the box trigger
 * @return The player
 */

AActor* UHidingComponent::GetPlayer() const
{
	// An array that will contain all the actors that are in the collider
	TArray<AActor*> PlayerActors;
	// Adds the actor that is in the collider to the array
	GetOverlappingActors(PlayerActors);
	
	// Goes through all the actors that are in the array
	for (AActor* Actor : PlayerActors)
	{
		// If the actor has the tag "Player" which only the player should have, returns that actor
		if (Actor->ActorHasTag("Player"))
		{
			APlayerCharacter* PC = Cast<APlayerCharacter>(Actor);

			if (PC != nullptr)
			{
				PC->SetHidingComponent(Cast<UHidingComponent>(GetOwner()->GetComponentByClass(UHidingComponent::StaticClass())));
			} 
			
			return Actor;
		}
	}
	// Otherwise return null
	return nullptr;
}

// void UHidingComponent::OpenDoor()
// {
// 	if (!LockerDoor) return;
// 	
// 	bOpen = true;
// 	
// 	// FRotator NewRotation = DoorRotation + FRotator(0, 140, 0);
// 	//
// 	// LockerDoor->SetRelativeRotation(FMath::RInterpConstantTo(DoorRotation, NewRotation, FApp::GetDeltaTime(), 10));
// }
//
// void UHidingComponent::CloseDoor()
// {
// 	bClose = true;
// }

