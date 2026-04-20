// Fill out your copyright notice in the Description page of Project Settings.


#include "StalkerMonsterCharacter.h"

#include "BlindMonsterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AStalkerMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	LookTimer += DeltaTime;
	if (LookTimer >= LookInterval)
	{
		LookTimer = 0.0f;
		bool bMonsterIsSeen = CheckIfPlayerIsLooking();
		
		ABlindMonsterAIController* BlindMonsterAIController = Cast<ABlindMonsterAIController>(GetController());
		if (BlindMonsterAIController && BlindMonsterAIController->GetBlackboardComponent())
		{
			BlindMonsterAIController->GetBlackboardComponent()->SetValueAsBool("IsDetected", bMonsterIsSeen);
		}
	}
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

bool AStalkerMonsterCharacter::CheckIfPlayerIsLooking()
{
	if (!PlayerPawn) return false;
	
	APlayerController* PlayerController = Cast<APlayerController>(PlayerPawn->GetController());
	if (!PlayerController) return false;
	
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * TraceScalar);
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerPawn); // Ingnore the player itself
	
	// Makes a SweepTrace every 0.1f so 10 times every second to see if it hits this monster, if so returns true.
	//We can change from Sweep to other like line if we dont want the error marginell
	FCollisionShape SphereTrace = FCollisionShape::MakeSphere(50.0f); // 50~1 meter width, could make variable for it
	if (GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity
		,ECollisionChannel::ECC_Visibility, SphereTrace, CollisionParams))
	{
		if (HitResult.GetActor() == this)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s hit"), *HitResult.GetActor()->GetName());
			return true;
		}
	}
	
	return false;
	
}