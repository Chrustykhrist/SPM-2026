// Fill out your copyright notice in the Description page of Project Settings.


#include "StalkerMonsterCharacter.h"

#include "StalkerMonsterAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/AudioComponent.h"
// Sets default values
AStalkerMonsterCharacter::AStalkerMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	
	AIControllerClass = AStalkerMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	if (SightConfig)
	{
		SightConfig->SightRadius = 3000.0f;
		SightConfig->LoseSightRadius = 3500.0f;
		SightConfig->PeripheralVisionAngleDegrees = 180.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		AIPerceptionComp->ConfigureSense(*SightConfig);
		AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}
	
	CurrentState = EStalkerMonsterCharacterState::Idle;
	
}

// Called when the game starts or when spawned
void AStalkerMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStalkerMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStalkerMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AStalkerMonsterCharacter::SetMonsterState(EStalkerMonsterCharacterState NewState)
{
	CurrentState = NewState;
}