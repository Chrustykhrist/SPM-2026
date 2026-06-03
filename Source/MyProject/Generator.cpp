// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "CustomPlayerState.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/SpotLight.h"

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh
	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>("GeneratorMesh");
	RootComponent = GeneratorMesh;
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	// Save the starting position of all doors
	if (!Openings.IsEmpty())
	{
		for (AActor* Actor : Openings)
		{
			StartLocations.Add(Actor->GetActorLocation());
		}
	}
	
	for (int i = 0; i < Openings.Num(); i++)
	{
		DoorYaws.Add(Openings[i]->GetActorRotation().Yaw);
		DoorOffsets.Add(0.0f);
	}
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Open all doors in chosen relative direction
	if (bOpen && !Openings.IsEmpty())
	{
		for (int i = 0; i < Openings.Num(); i++)
		{
			// FVector LocalOffset = Openings[i]->GetActorForwardVector() * MoveDistance.X
			// 					+ Openings[i]->GetActorRightVector() * MoveDistance.Y
			// 					+ Openings[i]->GetActorUpVector() * MoveDistance.Z;
			// FVector NewLocation = StartLocations[i] + LocalOffset;
			//
			// Openings[i]->SetActorLocation(FMath::VInterpConstantTo(Openings[i]->GetActorLocation(), NewLocation, DeltaTime, 30));
			
			FRotator TargetRotation = FRotator(0, DoorYaws[i] + DoorOffsets[i], 0);
			Openings[i]->SetActorRotation(FMath::RInterpConstantTo(Openings[i]->GetActorRotation(), TargetRotation, DeltaTime, 30));
		}
	}
}

void AGenerator::TurnOn()
{
	// Brighten all lights
	if (!SpotLights.IsEmpty())
	{
		for (ASpotLight* SpotLight : SpotLights)
		{
			SpotLight->SpotLightComponent->SetIntensity(SpotLight->SpotLightComponent->Intensity + IntensityIncrease);
			SpotLight->SpotLightComponent->AttenuationRadius += AttenuationIncrease;
		}
	}
	
	APawn* Pawn = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
  
	if (Pawn == nullptr) return;
  
	FVector PawnLocation = Pawn->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("OpenDoors| Before loop"));
	for (int i = 0; i < Openings.Num(); i++)
	{
		FVector DoorForward = FRotator(0, DoorYaws[i], 0).Vector();
		FVector ToDoor = (Openings[i]->GetActorLocation() - PawnLocation).GetSafeNormal();
     
		float Dot = FVector::DotProduct(DoorForward, ToDoor);
		DoorOffsets[i] = (Dot >= 0.0f) ? 90.0f : -90.0f;
		//UE_LOG(LogTemp, Warning, TEXT("OpenDoors| Inside loop"));
		// Save that this door was opened
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();
			if (PS)
			{
				PS->SetDoorOpened(FName(*Openings[i]->GetName()));
				//UE_LOG(LogTemp, Warning, TEXT("OpenDoors| PS SetDoorOpened %s"), *Doors[i]->GetName());
			}
		}
	}
	
	// Allow doors to move
	bOpen = true;
}

