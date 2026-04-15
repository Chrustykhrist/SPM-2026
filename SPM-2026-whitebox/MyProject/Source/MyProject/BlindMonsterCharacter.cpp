// Fill out your copyright notice in the Description page of Project Settings.


#include "BlindMonsterCharacter.h"
#include "BlindMonsterAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
// Sets default values
ABlindMonsterCharacter::ABlindMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = ABlindMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// Make sure it doesnt update dependent on controller roation
	bUseControllerRotationYaw = false;
	// Make it so the monster rotates orianted with the direction the monster moves
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Set the rotate speed on the Yaw
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSenser"));
	PawnSensingComponent->SightRadius = 0.0f;
	PawnSensingComponent->SetPeripheralVisionAngle(0.0f);
	PawnSensingComponent->HearingThreshold = 1200.0f;
}

// Called when the game starts or when spawned
void ABlindMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnHearNoise.AddDynamic(this, &ABlindMonsterCharacter::OnHearNoise);
	}
}

// Called every frame
void ABlindMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABlindMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABlindMonsterCharacter::OnHearNoise(APawn* OtherPawn, const FVector& Location, float Volume)
{
		if (OtherPawn != nullptr && OtherPawn != this && OtherPawn->IsPlayerControlled())
		{
			//UE_LOG(LogTemp, Warning, TEXT("OtherPawn är inte sig själv eller nullptr!"));
			ABlindMonsterAIController* AIController = Cast<ABlindMonsterAIController>(GetController());
			//UE_LOG(LogTemp, Warning, TEXT("AIController namn %s"), *AIController->GetClass()->GetName());
			if (AIController->GetBlackboardComponent() == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent är nullptr"))
			}
			//UE_LOG(LogTemp, Warning, TEXT("BlackboardComp: %s"), *AIController->GetBlackboardComponent()->GetName());
			if (AIController && AIController->GetBlackboardComponent())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Inne i sista if"));
				AIController->GetBlackboardComponent()->SetValueAsVector("TargetLocation", Location);
				AIController->GetBlackboardComponent()->SetValueAsBool("IsAlerted", false);
				AIController->GetBlackboardComponent()->SetValueAsBool("IsAlerted", true);
				UE_LOG(LogTemp, Warning, TEXT("Spelare hördes vid %s"), *Location.ToString());
			}
		}
}

