// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PickUp.generated.h"

class ACustomPlayerState;
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwapKeycard, AActor*, Keycard);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UPickUp : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickUp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Picks up the item that the player is looking at
	UFUNCTION()
	void PickUpItem();
	
	UPROPERTY(BlueprintAssignable, Category="Pick Up")
	FSwapKeycard SwapKeycard;
	
	UPROPERTY(EditAnywhere, Category="Display")
	TSubclassOf<UUserWidget> NotifClass;
	
	UPROPERTY()
	UUserWidget* Notif;
	
	// Variables of the held item
	
	UPROPERTY(BlueprintReadWrite)
	FName HeldTags;
	
	UFUNCTION(BlueprintCallable)
	void SetHeldTags(FName Tags) { HeldTags = Tags; }
	
	UFUNCTION(BlueprintCallable)
	FName GetHeldTags() { return HeldTags; }
	
	UPROPERTY(BlueprintReadWrite)
	FTransform HeldTransform;
	
	UFUNCTION(BlueprintCallable)
	void SetHeldTransform(FTransform Transform) { HeldTransform = Transform; }
	
	UFUNCTION(BlueprintCallable)
	FTransform GetHeldTransform() { return HeldTransform; }
	
	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* HeldMaterial;
	
	UFUNCTION(BlueprintCallable)
	void SetHeldMaterial(UMaterialInterface* Material) { HeldMaterial = Material; }
	
	UFUNCTION(BlueprintCallable)
	UMaterialInterface* GetHeldMaterial() { return HeldMaterial; }

private:
	UFUNCTION()
	void RemoveNotif();
	
	UFUNCTION()
	void CollectItem(const FHitResult& Hit, ACustomPlayerState* PlayerState);
	
	UFUNCTION()
	void ShowNotif();
	
	// The max distance allowed between the players eyes and the item. Measured in cm
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 100;
	
	// The radius of the sphere that hits the item
	UPROPERTY(EditAnywhere)
	float GrabRadius = 25;

	// Bool to check if the item is grabbable
	bool bGrabbable = false;
	
	UPROPERTY(EditAnywhere)
	float RestartDelay = 1.5f;
	
	UPROPERTY()
	FTimerHandle TimerHandle;
};
