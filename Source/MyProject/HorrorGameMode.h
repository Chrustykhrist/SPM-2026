// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HorrorGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AHorrorGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Game Rules")
	void PlayerDied();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	class USoundBase* DeathSound;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Game Rules")
	void OnPlayerDeathVisuals();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Game Rules")
	void RemoveVisuals();
	
	void GameOver() const;
	
private:
	UPROPERTY(EditAnywhere, Category = "Game Rules", meta = (AllowPrivateAccess = true))
	float RestartDelay = 3.f;
	
	FTimerHandle RestartTimerHandle;
};
