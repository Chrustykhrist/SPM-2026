// Fill out your copyright notice in the Description page of Project Settings.


#pragma once


#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SaveData.h"
#include "CustomPlayerState.generated.h"


/**
*
*/
UCLASS()
class MYPROJECT_API ACustomPlayerState : public APlayerState
{
   GENERATED_BODY()
  
public:
   ACustomPlayerState();
  
   virtual void BeginPlay() override;


   // Set the new spawn point
   UFUNCTION()
   void SetCheckPointTransform(FTransform NewSpawnTransform);


   // Get spawn point
   UFUNCTION()
   FTransform GetCheckPointTransform() const;
  
   UFUNCTION(BlueprintCallable)
   bool CheckForPowerKey();
  
   UFUNCTION(BlueprintCallable)
   bool HasRequiredItem(FName ItemName, int RequiredAmount = 1);
  
   UFUNCTION(BlueprintCallable)
   void UseItem(FName ItemName, int AmountUsed = 1);
  
   /*
    * Saves the game onto a file
    */
   void PopulateSaveData(FMasterSaveData& SaveData) const;
  
   void LoadFromSaveData(const FMasterSaveData& SaveData);
  
   UFUNCTION(BlueprintCallable, Category = "SaveSystem")
   void TriggerSaveGame(FString SlotName = TEXT("SaveSlot0"));
  
   UFUNCTION(BlueprintCallable, Category = "SaveSystem")
   void TriggerLoadGame(FString SlotName = TEXT("SaveSlot0"));
  
   void RestoreWorldState();
  
   UFUNCTION(BlueprintCallable)
   int GetItemCount(FName ItemName);
   
   UFUNCTION()
   TMap<FName, int> GetCollectedItems();
  
   // Inventory of the collected items, Key : Tag, Value : Number of items with that tag
   UPROPERTY()
   TMap<FName, int> CollectedItems;
  
   // Saved location to where the player should spawn after death
   UPROPERTY()
   FTransform SpawnTransform;
  
   UPROPERTY()
   TArray<FName> UnlockedDoors;


   UFUNCTION(BlueprintCallable)
   void UnlockDoor(FName DoorName);


   UFUNCTION(BlueprintCallable)
   bool IsDoorUnlocked(FName DoorName) const;
  
   UPROPERTY()
   TArray<FName> OpenedDoors;


   UFUNCTION(BlueprintCallable)
   void SetDoorOpened(FName DoorName);


   UFUNCTION(BlueprintCallable)
   bool IsDoorOpened(FName DoorName) const;
  
   UPROPERTY()
   TMap<FName, bool> KeypadStates;


   UPROPERTY()
   TMap<FName, FString> KeypadCodes;


   UFUNCTION(BlueprintCallable)
   void SetKeypadSolved(FName KeypadName, FString Code);


   UFUNCTION(BlueprintCallable)
   bool IsKeypadSolved(FName KeypadName) const;


   UFUNCTION(BlueprintCallable)
   FString GetKeypadCode(FName KeypadName) const;
};
