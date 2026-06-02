// Fill out your copyright notice in the Description page of Project Settings.




#include "CustomPlayerState.h"


#include <rapidjson/rapidjson.h>
#include "SaveManager.h"


#include "CustomGameInstance.h"
#include "KeycardReader.h"
#include "KeyPadComponent.h"
#include "ValveComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"


ACustomPlayerState::ACustomPlayerState()
{
   // Make places for the items that will be added
   CollectedItems.Add(FName("Battery"), 0);
   CollectedItems.Add(FName("Medicine"), 0);
   CollectedItems.Add(FName("PowerKey"), 0);
   CollectedItems.Add(FName("KeycardA"), 0);
   CollectedItems.Add(FName("KeycardB"), 0);
   CollectedItems.Add(FName("KeycardC"), 0);
   CollectedItems.Add(FName("KeycardD"), 0);
   CollectedItems.Add(FName("KeycardE"), 0);
   CollectedItems.Add(FName("Flashlight"), 0);
}


void ACustomPlayerState::BeginPlay()
{
   Super::BeginPlay();


   // Get and set the PlayerStart location for an initial spawn point
   // FVector Start = GetWorld()->GetAuthGameMode()->FindPlayerStart(GetWorld()->GetFirstPlayerController())->GetActorLocation();
   //
   // SpawnTransform.SetLocation(Start);
   // if (USaveManager::DoesSaveExist(TEXT("SaveSlot0")))
   // {
   //     TriggerLoadGame();
   // }
   // else
   // {
   //     // Get and set the PlayerStart location for an initial spawn point
   //     FVector Start = GetWorld()->GetAuthGameMode()
   //        ->FindPlayerStart(GetWorld()->GetFirstPlayerController())
   //        ->GetActorLocation();
   //     SpawnTransform.SetLocation(Start);
   // }
   // FVector Start = GetWorld()->GetAuthGameMode()->FindPlayerStart(GetWorld()->GetFirstPlayerController())->GetActorLocation();
   //
   //SpawnTransform.SetLocation(Start);
#if !WITH_EDITOR   
   if (USaveManager::DoesSaveExist(TEXT("SaveSlot0")))
   {
      TriggerLoadGame();
      
      // Move player to saved checkpoint
      APlayerController* PC = GetWorld()->GetFirstPlayerController();
      if (PC)
      {
         APawn* Player = PC->GetPawn();
         if (Player && !SpawnTransform.GetLocation().IsZero())
         {
            Player->SetActorLocationAndRotation(
               SpawnTransform.GetLocation(),
               SpawnTransform.GetRotation()
            );
         }
      }
   }
   else
   {
      FVector Start = GetWorld()->GetAuthGameMode()
         ->FindPlayerStart(GetWorld()->GetFirstPlayerController())
         ->GetActorLocation();
      SpawnTransform.SetLocation(Start);
   }
#else
   FVector Start = GetWorld()->GetAuthGameMode()
         ->FindPlayerStart(GetWorld()->GetFirstPlayerController())
         ->GetActorLocation();
   SpawnTransform.SetLocation(Start);
#endif 
   UCustomGameInstance* GI = Cast<UCustomGameInstance>(GetGameInstance());
  
   const TMap<FName, int>& Items = GI->GetInventory();
   CollectedItems.Append(Items);
}


/**
* Sets the new spawn position
*
* @param NewSpawnTransform
*/
void ACustomPlayerState::SetCheckPointTransform(FTransform NewSpawnTransform)
{
   SpawnTransform = NewSpawnTransform;
}


/**
* @return The current spawn point
*/
FTransform ACustomPlayerState::GetCheckPointTransform() const
{
   return SpawnTransform;
}


/**
* @return If we have the key or not
*/
bool ACustomPlayerState::CheckForPowerKey()
{
   if (CollectedItems[FName("PowerKey")] >= 1)
   {
      return true;
   }
  
   return false;
}


/**
* @return If we have the required item or not, so this method could be reused for the other items as well
*/
bool ACustomPlayerState::HasRequiredItem(FName ItemName, int RequiredAmount)
{
   if (CollectedItems.Contains(ItemName))
   {
      return CollectedItems[ItemName] >= RequiredAmount;
   }
  
   return false;
}


void ACustomPlayerState::UseItem(FName ItemName, int AmountUsed)
{
   if (CollectedItems.Contains(ItemName))
   {
      CollectedItems[ItemName] -= AmountUsed;
   }
}

int ACustomPlayerState::GetItemCount(FName ItemName)
{
   if (!CollectedItems.Contains(ItemName)) return 0;
  
   //UE_LOG(LogTemp, Warning, TEXT("Item Count: %d"), CollectedItems[ItemName]);
  
   return CollectedItems[ItemName];
}


// puts all the latest checkpoints and items into save file
void ACustomPlayerState::PopulateSaveData(FMasterSaveData& SaveData) const
{
   SaveData.SavedSpawnTransform = SpawnTransform;
   SaveData.SavedItems = CollectedItems;
   SaveData.UnlockedDoors = UnlockedDoors;
   SaveData.OpenedDoors = OpenedDoors;
   SaveData.KeypadStates = KeypadStates;
   SaveData.KeypadCodes = KeypadCodes;
   SaveData.CompletedValves = CompletedValves;
   FString LevelName = GetWorld()->GetMapName();
   LevelName.RemoveFromStart(TEXT("/Game/FirstPerson/"));
   SaveData.SavedLevel = LevelName;
   
   for (const FName& Door : OpenedDoors)
   {
      UE_LOG(LogTemp, Warning, TEXT("PopulateSaveData| Saving opened door: %s"), *Door.ToString());
   }
}


void ACustomPlayerState::LoadFromSaveData(const FMasterSaveData& SaveData)
{
   SpawnTransform = SaveData.SavedSpawnTransform;
  
   for (const auto& Element : SaveData.SavedItems)
   {
      CollectedItems.Emplace(Element.Key, Element.Value);
   }
   
   UnlockedDoors = SaveData.UnlockedDoors;
   OpenedDoors = SaveData.OpenedDoors;
   KeypadStates = SaveData.KeypadStates;
   KeypadCodes = SaveData.KeypadCodes;
   CompletedValves = SaveData.CompletedValves;
   
   for (const FName& Door : OpenedDoors)
   {
      UE_LOG(LogTemp, Warning, TEXT("LoadFromSaveData| Loaded opened door: %s"), *Door.ToString());
   }
}


void ACustomPlayerState::TriggerSaveGame(FString SlotName)
{
#if !WITH_EDITOR   
   FMasterSaveData NewSave;
   PopulateSaveData(NewSave);
  
   if (USaveManager::SaveGame(SlotName, NewSave))
   {
      UE_LOG(LogTemp, Log, TEXT("The game saved successfully"));
   }
#else
   UE_LOG(LogTemp, Warning, TEXT("TriggerSaveGame| Skipped in Editor"));
   UE_LOG(LogTemp, Warning, TEXT("MapName: %s"), *GetWorld()->GetMapName());
#endif 
}


void ACustomPlayerState::TriggerLoadGame(const FString SlotName)
{
#if !WITH_EDITOR   
   FMasterSaveData LoadedData;
   if (USaveManager::LoadGame(SlotName, LoadedData))
   {
      LoadFromSaveData(LoadedData);
      UE_LOG(LogTemp, Log, TEXT("The game loaded successfully"));
     
      RestoreWorldState();
   }
#else
   UE_LOG(LogTemp, Warning, TEXT("TriggerLoadGame| Skipped in Editor"));
#endif 
}


TMap<FName, int> ACustomPlayerState::GetCollectedItems()
{
   return CollectedItems;
}


void ACustomPlayerState::UnlockDoor(FName DoorName)
{
   UnlockedDoors.AddUnique(DoorName);
}


bool ACustomPlayerState::IsDoorUnlocked(FName DoorName) const
{
   return UnlockedDoors.Contains(DoorName);
}


void ACustomPlayerState::SetDoorOpened(FName DoorName)
{
   OpenedDoors.AddUnique(DoorName);
}


bool ACustomPlayerState::IsDoorOpened(FName DoorName) const
{
   return OpenedDoors.Contains(DoorName);
}


void ACustomPlayerState::SetKeypadSolved(FName KeypadName, FString Code)
{
   KeypadStates.Add(KeypadName, true);
   KeypadCodes.Add(KeypadName, Code);
}


bool ACustomPlayerState::IsKeypadSolved(FName KeypadName) const
{
   const bool* Found = KeypadStates.Find(KeypadName);
   return Found && *Found;
}


FString ACustomPlayerState::GetKeypadCode(FName KeypadName) const
{
   const FString* Found = KeypadCodes.Find(KeypadName);
   return Found ? *Found : TEXT("");
}

void ACustomPlayerState::SetValveCompleted(FName ValveName)
{
   CompletedValves.AddUnique(ValveName);
}

bool ACustomPlayerState::IsValveCompleted(FName ValveName) const
{
   return CompletedValves.Contains(ValveName);
}

void ACustomPlayerState::RestoreWorldState()
{
   // find all the keycard readers and restore them
   TArray<AActor*> Readers;
   UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeycardReader::StaticClass(), Readers);
   for (AActor* Actor : Readers)
   {
      AKeycardReader* Reader = Cast<AKeycardReader>(Actor);
      if (Reader)
      {
         Reader->RestoreDoorStates(this);
      }
   }
  
   // Restore all Keypads
   TArray<AActor*> AllActors;
   UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
   for (AActor* Actor : AllActors)
   {
      UKeyPadComponent* Keypad = Actor->FindComponentByClass<UKeyPadComponent>();
      if (Keypad) Keypad->RestoreState(this);
      
      UValveComponent* Valve = Actor->FindComponentByClass<UValveComponent>();
      if (Valve) Valve->RestoreState(this);
   }
}
