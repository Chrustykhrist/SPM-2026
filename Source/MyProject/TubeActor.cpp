// Fill out your copyright notice in the Description page of Project Settings.


#include "TubeActor.h"

// Sets default values
ATubeActor::ATubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FluidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FluidMesh"));
	RootComponent = FluidMesh;
}

// Called when the game starts or when spawned
void ATubeActor::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	// i know bad way of figureing out the distance the actor needs to travel,
	//just wanted to see if it works as i intended
	UnderTheMap = StartLocation - FVector(0, 0, 350);
	
	CurrentFillLevel = 1.0f;
}
// check if its draining if not make it drain
void ATubeActor::Drain()
{
	if (!bDraining)
	{
		bDraining = true;
	}
}

// Called every frame
// look if its draining and more than zero to then drain it every tick till zero and make it invisible
void ATubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bDraining && CurrentFillLevel > 0.0f)
	{
		
		// make it so the tube/substance moves slowly downards underneath the map and then
		// dissapears making it seem its draining
		
		// changes the currentfilllevel slower the more drainduration
		CurrentFillLevel -= DeltaTime / DrainDuration;
		// makes the position based of procentage so alpha 0 is start and 1 is underthemap
		float Alpha = FMath::Clamp(1.0f - CurrentFillLevel, 0.0f, 1.0f);
		FVector NewLocation = FMath::Lerp(StartLocation, 
			UnderTheMap, Alpha);
		
		SetActorLocation(NewLocation);
		
		// when its underthemap disable things we dont need anymore
		if (CurrentFillLevel <= 0.0f)
		{
			CurrentFillLevel = 0.0f;
			SetActorTickEnabled(false);
			this->Destroy();
		}
	}
}

