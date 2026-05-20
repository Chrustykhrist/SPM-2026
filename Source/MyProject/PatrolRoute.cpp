// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolRoute.h"

// Sets default values
APatrolRoute::APatrolRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

FVector APatrolRoute::GetWaypointsAverage() const
{
	if (Waypoints.Num() == 0) return FVector::ZeroVector;
	
	FVector Sum = FVector::ZeroVector;
	int Count = 0;
	
	for (const AActor* WP : Waypoints)
	{
		if (WP)
		{
			Sum += WP->GetActorLocation();
			Count++;
		}
	}
	
	return Count > 0 ? Sum / Count : FVector::ZeroVector;
}
