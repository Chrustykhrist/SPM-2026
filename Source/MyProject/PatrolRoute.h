// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolRoute.generated.h"

UCLASS()
class MYPROJECT_API APatrolRoute : public AActor
{
	GENERATED_BODY()

public:	
	APatrolRoute();
	
	// predetermined waypoints that we can place out as empty actors put them in the array
	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<AActor*> Waypoints;
	
	FVector GetWaypointsAverage() const;
};
