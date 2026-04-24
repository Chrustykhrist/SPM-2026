// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"

#include "CustomPlayerState.h"

// Sets default values for this component's properties
UPickUp::UPickUp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPickUp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPickUp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UPickUp::PickUp()
{
	FVector PlayerPos = GetComponentLocation();
	FVector GrabVector = PlayerPos + GetForwardVector() * MaxGrabDistance;

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), PlayerPos, GrabVector, FColor::Red);
#endif

	FHitResult ItemHit;

	FCollisionShape GrabVolume = FCollisionShape::MakeSphere(GrabRadius);

	bIsGrabbed = GetWorld()->SweepSingleByChannel(ItemHit, PlayerPos, GrabVector, FQuat::Identity, ECC_GameTraceChannel2, GrabVolume);
	
	if (bIsGrabbed)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();

		FName ItemName = ItemHit.GetActor()->Tags[0];

		if (PS->CollectedItems.Contains(ItemName))
		{
			PS->CollectedItems.Add(ItemName, PS->CollectedItems[ItemName] + 1);
		} else
		{
			PS->CollectedItems.Add(ItemName, 1);
		}

		ItemHit.GetActor()->Destroy();
	}
}

