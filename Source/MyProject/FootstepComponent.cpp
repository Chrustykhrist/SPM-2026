// Fill out your copyright notice in the Description page of Project Settings.


#include "FootstepComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "PlayerCharacter.h"

class APlayerCharacter;
// Sets default values for this component's properties
UFootstepComponent::UFootstepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFootstepComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ACharacter>(GetOwner());
	if (PlayerCharacter)
	{
		CharacterMovementComponent = PlayerCharacter->GetCharacterMovement();
	}
	
}


// Called every frame
void UFootstepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FootstepTick(DeltaTime);
}

void UFootstepComponent::SetMovementState(EMovementState NewMovementState)
{
	CurrentMovementState = NewMovementState;
	StepTimer = 0.0f;
}

void UFootstepComponent::SetIsMoving(bool NewIsMoving)
{
	bIsMoving = NewIsMoving;
	//if (!bIsMoving) StepTimer = 0.0f;
	if (PlayerCharacter->GetCharacterMovement()->Velocity == FVector::ZeroVector) StepTimer = 0.0f;
}

void UFootstepComponent::FootstepTick(float DeltaTime)
{
	//if (!bIsMoving || !PlayerCharacter) return;
	
	if (PlayerCharacter->GetCharacterMovement()->Velocity == FVector::ZeroVector || !PlayerCharacter) return;
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerCharacter))
		if (!bIsMoving) return;
	StepTimer += DeltaTime;
	
	float Interval = GetStepInterval(CurrentMovementState);
	if (StepTimer >= Interval)
	{
		StepTimer = 0.0f;
		ESurfaceType SurfaceType = DetectSurfaceType();
		PlayFootstep(SurfaceType, CurrentMovementState);
	}
}

ESurfaceType UFootstepComponent::DetectSurfaceType()
{
	if (!PlayerCharacter) return ESurfaceType::Default;
	
	UCapsuleComponent* CapsuleComponent = PlayerCharacter->GetCapsuleComponent();
	FVector Start = CapsuleComponent->GetComponentLocation();
	FVector End = Start - FVector(0.0f, 0.0f, CapsuleComponent->GetScaledCapsuleHalfHeight() + 30.0f);
	
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerCharacter);
	QueryParams.bReturnPhysicalMaterial = true;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, 
		ECollisionChannel::ECC_Visibility, QueryParams);
	
#if WITH_EDITOR	
	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 0.5f);
#endif	
	if (bHit && Hit.PhysMaterial.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Material hit: %s"), *Hit.PhysMaterial.Get()->GetName());
		UPhysicalMaterial* HitMaterial = Hit.PhysMaterial.Get();
		
		if (ConcreteMaterial && HitMaterial == ConcreteMaterial)
		{
			return ESurfaceType::Concrete;
		}
		if (GrassMaterial && HitMaterial == GrassMaterial)
		{
			return ESurfaceType::Grass;
		}
		
	}
	
	return ESurfaceType::Default;
}

void UFootstepComponent::PlayFootstep(ESurfaceType SurfaceType, EMovementState MovementState)
{
	UFMODEvent* EventToPlay = nullptr;
	
	switch (MovementState)
	{
		case EMovementState::Walking: EventToPlay = WalkEvent; break;
		case EMovementState::Sprinting: EventToPlay = SprintEvent; break;
		case EMovementState::Sneaking: EventToPlay = SneakEvent; break;
	}
	
	if (!EventToPlay) return;
	
	float SurfaceValue = 0.0f;
	switch (SurfaceType)
	{
		case ESurfaceType::Concrete: SurfaceValue = 0.0f; break;
		case ESurfaceType::Grass: SurfaceValue = 1.0f; break;
		default: SurfaceValue = 0.0f; break;
	}
	
	UE_LOG(LogTemp, Log, TEXT("FMOD EventTOPlay: %s"), *EventToPlay->GetName());
	
	// FFMODEventInstance Instance = UFMODBlueprintStatics::PlayEventAtLocation(
	// 	GetWorld(),
	// 	EventToPlay,
	// 	FTransform(PlayerCharacter->GetActorLocation()),
	// 	true);
	
	// if (MovementState == EMovementState::Walking && CharacterMovementComponent)
	// {
	// 	float Speed = CharacterMovementComponent->Velocity.Size();
	// 	UFMODBlueprintStatics::EventInstanceSetParameter(
	// 		Instance,
	// 		VelocityParameterName,
	// 		Speed);
	// }
	
	// UFMODBlueprintStatics::EventInstanceSetParameter(
	// 	Instance,
	// 	SurfaceParameterName,
	// 	SurfaceValue);
	
	// Make a UFMODAudioComp instead so it follows the mesh of the actor
	// therefore make a spatial sound
	USceneComponent* TargetToAttach = PlayerCharacter->GetMesh();
	UFMODAudioComponent* Instance = UFMODBlueprintStatics::PlayEventAttached(
		EventToPlay,
		TargetToAttach,
		NAME_None,               
		FVector::ZeroVector,    
		EAttachLocation::SnapToTarget,
		true,                    
		true,                 
		true);   
	
	if (!Instance) return;
	
	Instance->SetParameter(SurfaceParameterName, SurfaceValue);

	if (MovementState == EMovementState::Walking && CharacterMovementComponent)
	{
		float Speed = CharacterMovementComponent->Velocity.Size();
		Instance->SetParameter(VelocityParameterName, Speed);
	}
	
}

float UFootstepComponent::GetStepInterval(EMovementState MovementState) const
{
	switch (MovementState)
	{
		case EMovementState::Sneaking:  return SneakStepInterval;
		case EMovementState::Sprinting: return SprintStepInterval;
		default: return WalkStepInterval;
	}
}