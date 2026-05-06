// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FMODBlueprintStatics.h"
#include "FootstepComponent.generated.h"

class UCharacterMovementComponent;

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walking,
	Sprinting,
	Sneaking
};

UENUM(BlueprintType)
enum class ESurfaceType : uint8
{
	Concrete,
	Grass,
	Default
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFootstepComponent();
	
	void SetMovementState(EMovementState NewMovementState);
	
	void SetIsMoving(bool bIsMoving);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	EMovementState GetCurrentMovementState() const {return CurrentMovementState;}
private:
	void FootstepTick(float DeltaTime);
	
	ESurfaceType DetectSurfaceType();
	
	void PlayFootstep(ESurfaceType SurfaceType, EMovementState MovementState);
	
	float GetStepInterval(EMovementState MovementState) const;
	
	// FMOD Events
	UPROPERTY(EditDefaultsOnly, Category="FMOD")
	UFMODEvent* WalkEvent;
	
	UPROPERTY(EditDefaultsOnly, Category="FMOD")
	UFMODEvent* SprintEvent;
	
	UPROPERTY(EditDefaultsOnly, Category="FMOD")
	UFMODEvent* SneakEvent;
	
	// step intervals for the different events
	UPROPERTY(EditDefaultsOnly, Category="Footsteps")
	float WalkStepInterval = 0.40f;
	
	UPROPERTY(EditDefaultsOnly, Category="Footsteps")
	float SprintStepInterval = 0.20f;
	
	UPROPERTY(EditDefaultsOnly, Category="Footsteps")
	float SneakStepInterval = 0.60f;
	
	// physical materials that the trace can detect
	UPROPERTY(EditDefaultsOnly, Category="Footsteps")
	UPhysicalMaterial* ConcreteMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category="Footsteps")
	UPhysicalMaterial* GrassMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category="Footsteps")
	FName SurfaceParameterName = FName("Surface");
	
	UPROPERTY(EditDefaultsOnly, Category="Footsteps")
	FName VelocityParameterName = FName("Velocity");
	
	float StepTimer = 0.f;
	bool bIsMoving = false;
	EMovementState CurrentMovementState = EMovementState::Walking;
	
	ACharacter* PlayerCharacter = nullptr;
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
};
