// Fill out your copyright notice in the Description page of Project Settings.




#include "SaveManager.h"
#include "JsonUtilities.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


FString GetSavePath(const FString& SlotName)
{
	return FPaths::ProjectSavedDir() / TEXT("SaveGames") / SlotName + TEXT(".json");
}


bool USaveManager::SaveGame(const FString& SlotName, const FMasterSaveData& SaveData)
{
	FString JsonString;
	if (!FJsonObjectConverter::UStructToJsonObjectString(SaveData, JsonString, 0, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGame| could not serialize SaveData"));
		return false;
	}
  
	bool bSuccess = FFileHelper::SaveStringToFile(JsonString, *GetSavePath(SlotName));
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGame| Could not write to file: %s"), *GetSavePath(SlotName));
	}
	return bSuccess;
}


bool USaveManager::LoadGame(const FString& SlotName, FMasterSaveData& OutSaveData)
{
	FString Path = GetSavePath(SlotName);
  
	if (!FPaths::FileExists(Path))
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadGame| No savefile found: %s"), *Path);
		return false;
	}


	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *Path))
	{
		UE_LOG(LogTemp, Error, TEXT("LoadGame| Could not read file"));
		return false;
	}


	return FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &OutSaveData, 0, 0);
}

FString USaveManager::GetLevelByName(const FString& SlotName)
{
	FMasterSaveData LoadedData;
	if (LoadGame(SlotName, LoadedData))
	{
		return LoadedData.SavedLevel;
	}
	return TEXT("LVL1"); 
}


bool USaveManager::DoesSaveExist(const FString& SlotName)
{
	return FPaths::FileExists(GetSavePath(SlotName));
}


bool USaveManager::DeleteSave(const FString& SlotName)
{
	return IFileManager::Get().Delete(*GetSavePath(SlotName));
}
