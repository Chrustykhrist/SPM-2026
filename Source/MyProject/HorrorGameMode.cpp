// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorGameMode.h"

#include "CustomPlayerState.h"
#include "Kismet/GameplayStatics.h"

void AHorrorGameMode::PlayerDied()
{
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APawn* PlayerPawn = PC->GetPawn();
		if (PlayerPawn)
		{
			PlayerPawn->DisableInput(PC);
		}
	}
	
	if (DeathSound)
	{
		UGameplayStatics::PlaySound2D(this, DeathSound);
	}
	
	OnPlayerDeathVisuals();
	
	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AHorrorGameMode::GameOver, RestartDelay, false);
	
	// Needed for removal of visuals, does not work in GameOver method, not sure why
	RemoveVisuals();
}

/**
 * Respawns the player at the last gotten checkpoint
 */
void AHorrorGameMode::GameOver() const
{
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	// Makes sure that we get the player controller
	if (PC == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller is null"));
		return;
	}

	// Restarts the player
	GetWorld()->GetAuthGameMode()->RestartPlayer(PC);
	
	ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();
	APawn* NewPawn = PC->GetPawn();

	// Makes sure we get the player state
	if (PS == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player state is null"));
		return;
	}
	// Makes sure we get the player pawn
	if (NewPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player pawn is null"));
		return;
	}

	// Spawns the player at the check point
	if (!PS->GetCheckPointTransform().GetLocation().IsZero())
	{
		NewPawn->SetActorLocationAndRotation
		(
			PS->GetCheckPointTransform().GetLocation(),
			PS->GetCheckPointTransform().GetRotation()
		);
	}
	
	NewPawn->EnableInput(PC);
}
