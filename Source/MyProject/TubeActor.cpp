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
	// makes an instance of the material so we can change it for each tube
	// this was for to drain material and not move it down
	// DynamicFluidMaterial = FluidMesh->CreateAndSetMaterialInstanceDynamic(0);
	// if (DynamicFluidMaterial)
	// {
	// 	DynamicFluidMaterial->SetScalarParameterValue(FillLevelParameterName, 1.0f);
	// }
	
	// setup for to able to physically move the actor downards slowly to make it seem its draining
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
		// CurrentFillLevel = FMath::Max(0.0f, CurrentFillLevel - DeltaTime / DrainDuration);
		//
		// if (DynamicFluidMaterial)
		// {
		// 	DynamicFluidMaterial->SetScalarParameterValue(FillLevelParameterName, CurrentFillLevel);
		// }
		
		
		// if (CurrentFillLevel <= 0.0f)
		// {
		// 	FluidMesh->SetVisibility(false);
		// 	SetActorTickEnabled(false);
		// }
		// CurrentFillLevel -= DeltaTime / DrainDuration;
		// FVector NewLocation = FMath::VInterpTo(GetActorLocation(), 
		// 	UnderTheMap, DeltaTime, DrainDuration);
		
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
			FluidMesh->SetVisibility(false);
			SetActorTickEnabled(false);
		}
	}
}

