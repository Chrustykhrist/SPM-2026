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
	
	RemoveVisuals();
}

void AHorrorGameMode::GameOver() const
{
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	check(PC != nullptr);
	
	GetWorld()->GetAuthGameMode()->RestartPlayer(PC);
	
	ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();
	APawn* NewPawn = PC->GetPawn();
	
	check(PS != nullptr);
	check(NewPawn != nullptr);
	
	if (!PS->GetCheckPointTransform().GetLocation().IsZero())
	{
		NewPawn->SetActorLocationAndRotation(
			PS->GetCheckPointTransform().GetLocation(),
			PS->GetCheckPointTransform().GetRotation()
		);
	}
	
	NewPawn->EnableInput(PC);
}
