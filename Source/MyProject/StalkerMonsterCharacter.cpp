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
	StalkerMonsterAIController = Cast<AStalkerMonsterAIController>(GetController());
}

// Called every frame
void AStalkerMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PlayerPawn || !StalkerMonsterAIController) return;
	
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
	
	// Check if not attached and if monster distance is within the point where it should start to attach and is Stalking
	//UE_LOG(LogTemp, Warning, TEXT("DistanceToPlayer: %f and CurrentState: %s"), DistanceToPlayer, *UEnum::GetValueAsString(CurrentState));
	if (!bIsAttached && DistanceToPlayer <= AttachDistance && CurrentState == EStalkerMonsterCharacterState::Stalking)
	{
		bIsAttached = true;
		
		StalkerMonsterAIController->StopMovement();
		
		//if (AudioComp && !AudioComp->IsPlaying()) AudioComp->Play();
		//UE_LOG(LogTemp, Warning, TEXT("Attached true and sound"));
	}
	
	if (bIsAttached && CurrentState == EStalkerMonsterCharacterState::Stalking)
	{
		FVector AttachedLocation = PlayerPawn->GetActorLocation() - (PlayerPawn->GetActorForwardVector() * StalkDistance);
		FVector SmoothedLocation = FMath::VInterpTo(GetActorLocation(), AttachedLocation, DeltaTime, FollowRunSpeed);
		SetActorLocation(SmoothedLocation);
		
		FRotator PlayerRotation = PlayerPawn->GetActorRotation();
		FRotator SmoothMonsterRotation = FMath::RInterpTo(GetActorRotation(), PlayerRotation, DeltaTime, FollowRotationSpeed);
		SetActorRotation(SmoothMonsterRotation);
		UE_LOG(LogTemp, Warning, TEXT("Actively being stalked"));
		
		//Psuedo
		/*
		 *if KillTimer >= IntervalUntilKilling
		 *activate Killing state, CurrentState = Killing
		 *If player now looks at monster he kills/attacks
		 *KillAnywayasEvenThoPlayerHasNotLookedAtMonster >= IntervalKillAnyways
		 * 
		 * Note: If player looks before the Killtimer hits monster still flees and that part already works
		 * though you need to put a another Check in TriggerFlee that CurrentState != Killing
		 */
	}
	
	LookTimer += DeltaTime;
	if (LookTimer >= LookInterval)
	{
		LookTimer = 0.0f;
		//bool bMonsterIsSeen = CheckIfPlayerIsLooking();
		
		if (StalkerMonsterAIController->GetBlackboardComponent())
		{
			// For Flee check if player sees monster and monster is not in Killing mode
			if (CheckIfPlayerIsLooking())
			{
				SetMonsterState((EStalkerMonsterCharacterState::Fleeing));
				bIsAttached = false;
				StalkerMonsterAIController->TriggerFlee();
				// SetMonsterState(EStalkerMonsterCharacterState::Fleeing);
				// StalkerMonsterAIController->GetBlackboardComponent()->SetValueAsBool("IsDetected", true);
				// StalkerMonsterAIController->GetBlackboardComponent()->SetValueAsEnum("MonsterState", (uint8) EStalkerMonsterCharacterState::Fleeing);
			}else
			{
				// If lets the flee sequence finish before stalk otherwise it will as soon as monster flees
				// ChechIfPlayerIsLooking can be false therefore make enum to Stalking and other requirements can also
				// be fulfilled (bIsAttached false and PlayerDistance <= AttachDistance)
				if (StalkerMonsterAIController->bIsFleeing) return;
				SetMonsterState(EStalkerMonsterCharacterState::Stalking);
				StalkerMonsterAIController->TriggerStalk();
			}
			
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