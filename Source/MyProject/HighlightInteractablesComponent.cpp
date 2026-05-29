#include "HighlightInteractablesComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MeshComponent.h"

UHighlightInteractablesComponent::UHighlightInteractablesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f; 
}

void UHighlightInteractablesComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (AActor* Owner = GetOwner())
	{
		Owner->GetComponents<UMeshComponent>(MeshComponents);
		UE_LOG(LogTemp, Warning, TEXT("UHighlightComponent::BeginPlay()"))
	}
}

void UHighlightInteractablesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PlayerPawn || MeshComponents.Num() == 0) return;
	
	float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetOwner()->GetActorLocation());

	if (Distance <= MaxDistance)
	{
		if (!bIsHighlighted)
		{
			EnableHighlight(true);
		}
	}
	else
	{
		if (bIsHighlighted)
		{
			EnableHighlight(false);
		}
	}
}

void UHighlightInteractablesComponent::EnableHighlight(bool bEnable)
{
	bIsHighlighted = bEnable;
	for (UMeshComponent* Mesh : MeshComponents)
	{
		if (Mesh)
		{
			Mesh->SetRenderCustomDepth(bEnable);
			
			if (bEnable)
			{
				Mesh->SetCustomDepthStencilValue(CustomDepthStencilValue);
			}
		}
	}
}
