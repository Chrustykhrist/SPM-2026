// Fill out your copyright notice in the Description page of Project Settings.


#include "ValveComponent.h"
#include "TubeActor.h"
// Sets default values for this component's properties
UValveComponent::UValveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UValveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UValveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UValveComponent::BeginInteraction()
{
	if (bComplete) return;
	bActive = true;
}

void UValveComponent::ApplyRotationDelta(float Delta)
{
	if (!bActive || bComplete) return;
	
	CurrentRotation = FMath::Clamp(CurrentRotation + Delta * InputSensitivity * 
		GetWorld()->GetDeltaSeconds(), 0.0f, RequiredRotationDegrees);
	
	// rotate the mesh of the valve physically on its own local z axis
	if (ValveMesh)
	{
		ValveMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 
			FMath::GetMappedRangeValueClamped(
				FVector2D(0.0f, RequiredRotationDegrees),
				FVector2D(0.0f, 360.0f), 
				CurrentRotation)));
	}
	
	if (CurrentRotation >= RequiredRotationDegrees)
	{
		CompleteValve();
	}
}

void UValveComponent::EndInteraction()
{
	bActive = false;
}

void UValveComponent::CompleteValve()
{
	bComplete = true;
	bActive = false;
	
	if (LinkedTube)
	{
		LinkedTube->Drain();
	}
	OnValveCompleted.Broadcast();
}
