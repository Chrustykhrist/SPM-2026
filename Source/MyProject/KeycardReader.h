// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeycardReader.generated.h"

class ACustomPlayerState;

UCLASS()
class MYPROJECT_API AKeycardReader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeycardReader();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Keycard")
	FName RequiredKeycard = FName("KeycardA");
	
	UFUNCTION(BlueprintCallable, Category = "Keycard")
	bool TryUnlock(ACustomPlayerState* PS);
	
	UFUNCTION(BlueprintCallable, Category = "Keycard")
	void OpenDoors();
	
	UPROPERTY(EditAnywhere, Category = "Keycard")
	TArray<AActor*> LinkedDoor;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<float> DoorYaws;
	TArray<float> DoorOffsets;
	
	bool bOpenDoors = false;
};
