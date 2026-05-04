// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"
// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* UInteractionComponent::FindInteractingActor() const
{
	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = StartLocation + GetForwardVector() * MaxInteractionDistance;
	
	FHitResult ActorHit;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRadius);
	
	//bool bHit = GetWorld()->SweepSingleByChannel(ActorHit, StartLocation, 
		//EndLocation, FQuat::Identity, ECC_GameTraceChannel13, Sphere);
	
	bool bHit = GetWorld()->SweepSingleByChannel(ActorHit, StartLocation, 
		EndLocation, FQuat::Identity, ECC_Visibility, Sphere);
	
	// if (bHit && ActorHit.GetActor() && 
	// 	ActorHit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	// {
	// 	return ActorHit.GetActor(); ECC_Visibility
	// }
	AActor* HitActor = ActorHit.GetActor();
	DrawDebugSphere(GetWorld(), EndLocation, InteractionRadius, 12, FColor::Red, false, 2.0f);
	if (bHit && HitActor)
	{
		if (HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			return HitActor;
		}
        
		UE_LOG(LogTemp, Warning, TEXT("Hit %s but it has no interactable interface"), *HitActor->GetName());
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("FindInteractingActor did not hit anything."));
	}
	
	return nullptr;
}

void UInteractionComponent::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Begin interact InteractionComponent %s"), bIsInteracting ? TEXT("true") : TEXT("false"));
	if (bIsInteracting) return;
	UE_LOG(LogTemp, Warning, TEXT("Begin interact InteractionComponent"));
	AActor* TargetActor = FindInteractingActor();
	UE_LOG(LogTemp, Warning, TEXT("Begin interact InteractionComponent, Actor: %s"), TargetActor ? *TargetActor->GetName() : TEXT("None"));
	if (!TargetActor) return;
	
	IInteractable* InteractableActor = Cast<IInteractable>(TargetActor);
	if (InteractableActor && InteractableActor->CanInteract())
	{
		CurrentInteractingActor = TargetActor;
		bIsInteracting = true;
		InteractableActor->OnInteractBegin(GetWorld()->GetFirstPlayerController());
	}
}

void UInteractionComponent::InteractHeld(float Delta)
{
	if (!bIsInteracting || !CurrentInteractingActor) return;
	UE_LOG(LogTemp, Warning, TEXT("Interact Held"));
	IInteractable* InteractableActor = Cast<IInteractable>(CurrentInteractingActor);
	if (InteractableActor)
	{
		InteractableActor->OnInteractHold(GetWorld()->GetFirstPlayerController(), Delta);
	}
}

void UInteractionComponent::EndInteract()
{
	if (!bIsInteracting || !CurrentInteractingActor) return;
	
	IInteractable* InteractableActor = Cast<IInteractable>(CurrentInteractingActor);
	if (InteractableActor)
	{
		InteractableActor->OnInteractEnd(GetWorld()->GetFirstPlayerController());
	}
	
	CurrentInteractingActor = nullptr;
	bIsInteracting = false;
}
