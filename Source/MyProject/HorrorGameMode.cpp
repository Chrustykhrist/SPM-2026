// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorGameMode.h"

#include "BlindMonsterCharacter.h"
#include "CustomPlayerState.h"
#include "FootstepComponent.h"
#include "PlayerCharacter.h"
#include "StalkerMonsterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AHorrorGameMode::PlayerDied(FVector KillerInstigator, ABlindMonsterCharacter* Monster)
{
	CachedDeathCharacter = Monster;
	// we could cache this in beginplay but if simulate instead of play in editor it may crash
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APawn* PlayerPawn = PC->GetPawn();
		if (PlayerPawn)
		{
			
			if (bSmoothDeathTurn)
			{
				// Smoothly blend the player's camera view to the monster actor over 1.5 seconds
				PC->SetViewTargetWithBlend(Monster, 1.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
			else
			{
				// Instantly snap the camera view to the monster
				PC->SetViewTarget(Monster);
			}
			// PlayerPawn->DisableInput(PC);
			// // snap the camera to the monster that killed the player before jumpscare with snap or smooth turn depending on bool bSmoothDeathTurn
			// if (!KillerInstigator.IsZero() && bSmoothDeathTurn)
			// {
			// 	FVector ToKiller = (KillerInstigator - PlayerPawn->GetActorLocation()).GetSafeNormal();
			// 	FRotator LookAt = ToKiller.Rotation();
			// 	LookAt.Pitch = PC->GetControlRotation().Pitch;
			// 	LookAt.Roll = 0.0f;
			// 	
			// 	CachedDeathPC = PC;
			// 	DeathTurnStartRotation = PC->GetControlRotation();
			// 	DeathTurnTargetRotation = LookAt;
			// 	DeathTurnElapsed = 0.0f;
			//
			// 	// tick ~60 times/sec for the duration
			// 	GetWorldTimerManager().SetTimer(
			// 		DeathTurnTimerHandle,
			// 		this,
			// 		&AHorrorGameMode::TickDeathTurn,
			// 		1.0f / 60.0f,
			// 		true
			// 	);
			// 	return; 
			}

			// if bSmoothDeathTurn is diabled or no killer it will fallback to snap turn instead
			// if (!KillerInstigator.IsZero())
			// {
			// 	FVector ToKiller = (KillerInstigator - PlayerPawn->GetActorLocation()).GetSafeNormal();
			// 	FRotator LookAt = ToKiller.Rotation();
			// 	LookAt.Pitch = PC->GetControlRotation().Pitch;
			// 	LookAt.Roll = 0.0f;
			// 	PC->SetControlRotation(LookAt);
			// }
		}
		StartDeathSequence();
	
	// if (DeathSound)
	// {
	// 	UGameplayStatics::PlaySound2D(this, DeathSound);
	// }
	//
	// OnPlayerDeathVisuals();
	//
	// GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AHorrorGameMode::GameOver, RestartDelay, false);
	//
	// APlayerCharacter* PCH = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//
	// //PCH->SetOnPauseStartScreen(false);
	//
	// if (PCH->bIsCrouched)
	// {
	// 	PCH->UnCrouch();
	// }
	//
	// if (PCH->GetMoving())
	// {
	// 	PCH->GetFootstepComponent()->SetIsMoving(false);
	// }
	//
	// PCH->ResetPlayer();
	}
	
	

void AHorrorGameMode::TickDeathTurn()
{
	DeathTurnElapsed += 1.0f / 60.0f;
	float Alpha = FMath::Clamp(DeathTurnElapsed / DeathTurnDuration, 0.0f, 1.0f);

	// able to make a optional curve for easing, otherwise its jsut linear
	float EasedAlpha = DeathTurnCurve
		? DeathTurnCurve->GetFloatValue(Alpha)
		: Alpha;

	if (CachedDeathPC)
	{
		FRotator BlendedRotation = FMath::Lerp(DeathTurnStartRotation, DeathTurnTargetRotation, EasedAlpha);
		CachedDeathPC->SetControlRotation(BlendedRotation);
	}

	if (Alpha >= 1.0f)
	{
		GetWorldTimerManager().ClearTimer(DeathTurnTimerHandle);
		StartDeathSequence();
	}
}

void AHorrorGameMode::StartDeathSequence()
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySound2D(this, DeathSound);
	}

	OnPlayerDeathVisuals();
	if (UAnimInstance* AnimInstance = CachedDeathCharacter->GetMesh()->GetAnimInstance())
	{
		if (JumpscareMontage)
		{
			AnimInstance->Montage_Play(JumpscareMontage);
		}
	}
	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AHorrorGameMode::GameOver, RestartDelay, false);

	APlayerCharacter* PCH = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PCH->bIsCrouched)
	{
		PCH->UnCrouch();
	}

	if (PCH->GetMoving())
	{
		PCH->GetFootstepComponent()->SetIsMoving(false);
	}

	PCH->ResetPlayer();
	
	if (UCharacterMovementComponent* CMC = Cast<UCharacterMovementComponent>(PCH->GetMovementComponent()))
	{
		CMC->GravityScale = 1.0f;
		CMC->SetMovementMode(MOVE_Walking);
	}
	
	PCH->SetActorEnableCollision(true);
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
	TArray<AStalkerMonsterCharacter*> StalkerMonsterActors;
	TArray<AActor*> AllMonsters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), OutActors);
	for (AActor* Actor : OutActors)
	{
		//ABlindMonsterCharacter* BlindMonster = Cast<ABlindMonsterCharacter>(Actor);
		if (Actor->IsA(ABlindMonsterCharacter::StaticClass())) AllMonsters.Add(Cast<ABlindMonsterCharacter>(Actor));
		if (Actor->IsA(AStalkerMonsterCharacter::StaticClass())) AllMonsters.Add(Cast<AStalkerMonsterCharacter>(Actor));
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
		PC->SetControlRotation(PS->GetCheckPointTransform().GetRotation().Rotator());
	}
	
	NewPawn->EnableInput(PC);
	
	// If we found blind monster reset its movement
	if (!AllMonsters.IsEmpty())
	{
		for (auto* CurrentMonster : AllMonsters)
		{
			if (CurrentMonster->IsA(ABlindMonsterCharacter::StaticClass()))
			{
				ABlindMonsterCharacter* BlindMonster = Cast<ABlindMonsterCharacter>(CurrentMonster);
				BlindMonster->ResetMovement();
				BlindMonster->Respawn();
			}
			else
			{
				AStalkerMonsterCharacter* StalkerMonster = Cast<AStalkerMonsterCharacter>(CurrentMonster);
				StalkerMonster->ResetMovement();
				StalkerMonster->Respawn();
			}
			
		}
		//BlindMonster->ResetMovement();
	}
}
