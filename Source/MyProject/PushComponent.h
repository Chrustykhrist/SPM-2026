// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PushComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UPushComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPushComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION( BlueprintCallable )
	void Push();
	
	UFUNCTION(BlueprintCallable)
	bool GetPowerswitched();
	
	UPROPERTY()
	FName RecentlyPressed;
	
private:
	UPROPERTY(EditAnywhere)
	float MaxPushDistance = 150;
	
	bool bPushable = false;
	
	bool bPowerSwitchPushed = false;
};
