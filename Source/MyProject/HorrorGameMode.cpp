// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorGameMode.h"

#include "BlindMonsterCharacter.h"
#include "CustomPlayerState.h"
#include "FootstepComponent.h"
#include "PlayerCharacter.h"
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
	
	APlayerCharacter* PCH = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	PCH->SetOnPauseStartScreen(false);
	
	if (PCH->bIsCrouched)
	{
		PCH->UnCrouch();
	}
	
	if (PCH->GetMoving())
	{
		PCH->GetFootstepComponent()->SetIsMoving(false);
	}
}

/**
 * Respawns the player at the last gotten checkpoint
 */
void AHorrorGameMode::GameOver()
{
	RemoveVisuals();
	
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
	
	// Find the blind monster
	//for (AActor* CurrentMonster )
	TArray<AActor*> OutActors;
	TArray<ABlindMonsterCharacter*> BlindMonsterActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlindMonsterCharacter::StaticClass(), OutActors);
	for (AActor* Actor : OutActors)
	{
		//ABlindMonsterCharacter* BlindMonster = Cast<ABlindMonsterCharacter>(Actor);
		BlindMonsterActors.Add(Cast<ABlindMonsterCharacter>(Actor));
	}
	//AActor* MonsterActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABlindMonsterCharacter::StaticClass());
	//ABlindMonsterCharacter* BlindMonster = Cast<ABlindMonsterCharacter>(MonsterActor);
	
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
	
	// If we found blind monster reset its movement
	if (!BlindMonsterActors.IsEmpty())
	{
		for (ABlindMonsterCharacter* CurrentMonster : BlindMonsterActors)
		{
			CurrentMonster->ResetMovement();
		}
		//BlindMonster->ResetMovement();
	}
}
