// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCustomGameInstance();
	
	UFUNCTION()
	TMap<FName, int> GetInventory();
	
	UFUNCTION(BlueprintCallable)
	bool GetPowerStatus() const { return bPowerIsOn; }
	
	UFUNCTION(BlueprintCallable)
	void SetPowerStatus(bool bPowerStatus) { bPowerIsOn = bPowerStatus; }
	
	UFUNCTION(BlueprintCallable)
	void SaveInventory();
	
protected:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnSeamlessTravelDuringReplay() override;
	
private:
	UPROPERTY()
	TMap<FName, int> Inventory;
	
	bool bPowerIsOn = false;
};
