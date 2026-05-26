// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/SpotLight.h"

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>("GeneratorMesh");
	RootComponent = GeneratorMesh;
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	if (!Openings.IsEmpty())
	{
		for (AActor* Actor : Openings)
		{
			StartLocations.Add(Actor->GetActorLocation());
		}
	}
	
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (bOpen && !Openings.IsEmpty())
	{
		for (int i = 0; i < Openings.Num(); i++)
		{
			FVector LocalOffset = Openings[i]->GetActorForwardVector() * MoveDistance.X
								+ Openings[i]->GetActorRightVector() * MoveDistance.Y
								+ Openings[i]->GetActorUpVector() * MoveDistance.Z;
			FVector NewLocation = StartLocations[i] + LocalOffset;
			
			Openings[i]->SetActorLocation(FMath::VInterpConstantTo(Openings[i]->GetActorLocation(), NewLocation, DeltaTime, 30));
		}
	}
}

void AGenerator::TurnOn()
{
	if (!SpotLights.IsEmpty())
	{
		for (ASpotLight* SpotLight : SpotLights)
		{
			SpotLight->SpotLightComponent->SetIntensity(SpotLight->SpotLightComponent->Intensity + IntensityIncrease);
			SpotLight->SpotLightComponent->AttenuationRadius += AttenuationIncrease;
		}
	}
	
	bOpen = true;
}

