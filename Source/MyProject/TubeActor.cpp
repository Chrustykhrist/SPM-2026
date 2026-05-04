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
	DynamicFluidMaterial = FluidMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicFluidMaterial)
	{
		DynamicFluidMaterial->SetScalarParameterValue(FillLevel, 1.0f);
	}
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
		CurrentFillLevel = FMath::Max(0.0f, CurrentFillLevel - DeltaTime / DrainDuration);
		
		if (DynamicFluidMaterial)
		{
			DynamicFluidMaterial->SetScalarParameterValue(FillLevel, CurrentFillLevel);
		}
		
		if (CurrentFillLevel <= 0.0f)
		{
			FluidMesh->SetVisibility(false);
			SetActorTickEnabled(false);
		}
	}
}

