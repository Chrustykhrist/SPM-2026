// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
//#include "BlindMonsterCharacter.h"
#include "HorrorGameMode.generated.h"
/**
 * 
 */
class ABlindMonsterCharacter;
UCLASS()
class MYPROJECT_API AHorrorGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Game Rules")
	void PlayerDied(FVector KillerInstigator = FVector::ZeroVector, ABlindMonsterCharacter* Monster = nullptr);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	class USoundBase* DeathSound;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Game Rules")
	void OnPlayerDeathVisuals();

	// Removes the jumpscare from the screen
	UFUNCTION(BlueprintImplementableEvent, Category = "Game Rules")
	void RemoveVisuals();
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* JumpscareMontage;
	
#pragma region TurnPlayerTowardsMonster	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Settings")
	bool bSmoothDeathTurn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Settings",
			  meta = (EditCondition = "bSmoothDeathTurn", ClampMin = "0.1", ClampMax = "3.0"))
	float DeathTurnDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Settings",
			  meta = (EditCondition = "bSmoothDeathTurn"))
	UCurveFloat* DeathTurnCurve;
#pragma endregion
	
	void GameOver();
	
private:
	UPROPERTY(EditAnywhere, Category = "Game Rules", meta = (AllowPrivateAccess = true))
	float RestartDelay = 3.f;
	
	FTimerHandle RestartTimerHandle;
	
	void StartDeathSequence();
	
	void TickDeathTurn();

	FTimerHandle DeathTurnTimerHandle;
	
	FRotator DeathTurnStartRotation;
	
	FRotator DeathTurnTargetRotation;
	
	float DeathTurnElapsed = 0.0f;
	
	APlayerController* CachedDeathPC = nullptr;
	
	ABlindMonsterCharacter* CachedDeathCharacter = nullptr;

};
