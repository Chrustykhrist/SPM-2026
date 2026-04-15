// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorGameMode.h"

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
	
}

void AHorrorGameMode::GameOver() const
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
