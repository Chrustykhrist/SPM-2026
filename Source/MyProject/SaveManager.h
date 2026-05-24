// Fill out your copyright notice in the Description page of Project Settings.


#pragma once


#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveData.h"
#include "SaveManager.generated.h"


/**
*
*/
UCLASS()
class MYPROJECT_API USaveManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
  
public:
	UFUNCTION(BlueprintCallable, Category = "Save Data")
	static bool SaveGame(const FString& SlotName, const FMasterSaveData& SaveData);
  
	UFUNCTION(BlueprintCallable, Category = "Save Data")
	static bool LoadGame(const FString& SlotName, FMasterSaveData& OutSaveData);
  
	UFUNCTION(BlueprintCallable, Category = "Save Data")
	static bool DoesSaveExist(const FString& SlotName);
  
	UFUNCTION(BlueprintCallable, Category = "Save Data")
	static bool DeleteSave(const FString& SlotName);
};
