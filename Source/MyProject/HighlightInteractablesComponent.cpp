#include "HighlightInteractablesComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MeshComponent.h"

UHighlightComponent::UHighlightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f; // Optimerat: Tickar bara 10 gånger i sekunden istället för varje frame
}

void UHighlightComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cachea spelarens pawn för att slippa söka efter den i varje Tick
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Hämta alla Mesh-komponenter på Actorn (både StaticMesh och SkeletalMesh)
	if (AActor* Owner = GetOwner())
	{
		Owner->GetComponents<UMeshComponent>(MeshComponents);
		UE_LOG(LogTemp, Warning, TEXT("UHighlightComponent::BeginPlay()"))
	}
}

void UHighlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PlayerPawn || MeshComponents.Num() == 0) return;

	// Beräkna avstånd mellan spelaren och objektet
	float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetOwner()->GetActorLocation());

	if (Distance <= MaxDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHighlightComponent::Distance good"))
		if (!bIsHighlighted)
		{
			EnableHighlight(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UHighlightComponent::Distance bad"))
		if (bIsHighlighted)
		{
			EnableHighlight(false);
		}
	}
}

void UHighlightComponent::EnableHighlight(bool bEnable)
{
	bIsHighlighted = bEnable;
	UE_LOG(LogTemp, Warning, TEXT("UHighlightComponent::EnableHighlight"))
	for (UMeshComponent* Mesh : MeshComponents)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHighlightComponent::EnableHighlight Looking for meshes"))
		if (Mesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("UHighlightComponent::EnableHighlight Mesh found"))
			// Aktiverar/inaktiverar Custom Depth rendering
			Mesh->SetRenderCustomDepth(bEnable);
			
			if (bEnable)
			{
				// Sätter stencil-värdet så att materialet kan läsa av det för kosmetiska skillnader
				UE_LOG(LogTemp, Warning, TEXT("UHighlightComponent::EnableHighlight bEnable true"))
				Mesh->SetCustomDepthStencilValue(CustomDepthStencilValue);
			}
		}
	}
}
