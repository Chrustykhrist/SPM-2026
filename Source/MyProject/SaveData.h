#pragma once


#include "CoreMinimal.h"
#include "SaveData.generated.h"


USTRUCT(BlueprintType)
struct FMasterSaveData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	int32 SaveVersion = 1;
  
	// saves the latest checkpoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	FTransform SavedSpawnTransform;
  
	// saves all the items and the amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	TMap<FName, int> SavedItems;
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> UnlockedDoors;
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> OpenedDoors;
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, bool> KeypadStates;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FString> KeypadCodes;
	// could add more things to save later on
};
