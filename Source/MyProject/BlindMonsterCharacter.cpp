// Fill out your copyright notice in the Description page of Project Settings.


#include "BlindMonsterCharacter.h"
#include "BlindMonsterAIController.h"
#include "HorrorGameMode.h"
#include "PlayerCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "HidingComponent.h" 

#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABlindMonsterCharacter::ABlindMonsterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DeathBoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathBoxTrigger"));
	DeathBoxTrigger->SetupAttachment(RootComponent);
	
	DeathBoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABlindMonsterCharacter::OnOverlapBegin);
	AIControllerClass = ABlindMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// Make sure it doesnt update dependent on controller roation
	bUseControllerRotationYaw = false;
	// Make it so the monster rotates orianted with the direction the monster moves
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Set the rotate speed on the Yaw
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	
	// Creates a UPawnSensingComponent at all instances of this class
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSenser"));
	PawnSensingComponent->SightRadius = 0.0f;
	PawnSensingComponent->SetPeripheralVisionAngle(0.0f);
	PawnSensingComponent->HearingThreshold = 2000.0f;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

// Called when the game starts or when spawned
void ABlindMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensingComponent)
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		PawnSensingComponent->OnHearNoise.AddDynamic(this, &ABlindMonsterCharacter::OnHearNoise);
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	}
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Player) return;
	HidingComp = Player->FindComponentByClass<UHidingComponent>();
}

// Called every frame
void ABlindMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckTimer += DeltaTime;
	if (CheckTimer >= CheckSightInterval)
	{
		CheckLineOfSight();
	}
	
}

// Called to bind functionality to input
void ABlindMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABlindMonsterCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		//Check if player is in the locker
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			HidingComp = Player->FindComponentByClass<UHidingComponent>();
			if (HidingComp && HidingComp->bHiding)
			{
				UE_LOG(LogTemp, Warning, TEXT("Monster tried to attack in the locker"));
				return; 
			}
		}
		AHorrorGameMode* GameMode = Cast<AHorrorGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GetCharacterMovement()->DisableMovement();
			GameMode->PlayerDied();
		}
	}
}

void ABlindMonsterCharacter::CheckLineOfSight()
{
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	CheckTimer = 0;
	if (!Player) return;
 
	// ignore player if they are hiding
	
	if (HidingComp && HidingComp->bHiding) 
	{
		// Maybe change to investigate near the place the player hid
		if (bIsChasing)
		{
			bIsChasing = false;
			ABlindMonsterAIController* AIC = Cast<ABlindMonsterAIController>(GetController());
			if (AIC && AIC->GetBlackboardComponent())
			{
				UE_LOG(LogTemp, Warning, TEXT("Was chasing but now player is hiding"));
				AIC->GetBlackboardComponent()->SetValueAsBool("IsChasing", false);
			}
		}
		return;
	}
 
	// distance check of player
	FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
	float Distance = ToPlayer.Size();
	if (Distance > SightDistance) 
	{
		UE_LOG(LogTemp, Warning, TEXT("player out of range distance player distance %f and SightDistance %f"), Distance, SightDistance);
		// if player leave sightdistance we stop chase
		if (bIsChasing)
		{
			bIsChasing = false;
			ABlindMonsterAIController* AIC = Cast<ABlindMonsterAIController>(GetController());
			if (AIC && AIC->GetBlackboardComponent())
			{
				UE_LOG(LogTemp, Warning, TEXT("Was chasing but now player to far away"));
				AIC->GetBlackboardComponent()->SetValueAsBool("IsChasing", false);
			}
		}
		return;
	}
 
	// degree check with dot product
	float AngleDeg = FMath::RadiansToDegrees(
		FMath::Acos(
			FVector::DotProduct(GetActorForwardVector(), ToPlayer.GetSafeNormal())));
 
	if (AngleDeg > SightAngle)
	{
		// the player is out of sight angle
		UE_LOG(LogTemp, Warning, TEXT("out of angle"));
		if (bIsChasing)
		{
			bIsChasing = false;
			ABlindMonsterAIController* AIC = Cast<ABlindMonsterAIController>(GetController());
			if (AIC && AIC->GetBlackboardComponent())
			{
				UE_LOG(LogTemp, Warning, TEXT("Was chasing but now out of angle"));
				AIC->GetBlackboardComponent()->SetValueAsBool("IsChasing", false);
			}
		}
		return;
	}
 
	// raycast to see if anything is in the way, for example a wall
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
 
	bool bBlocked = GetWorld()->LineTraceSingleByChannel(
		Hit,
		GetActorLocation(),
		Player->GetActorLocation(),
		ECC_Visibility,
		Params);
 
	// if not player we ignore
	if (bBlocked && Hit.GetActor() != Player) return;
	
	if (!bIsChasing)
	{
		bIsChasing = true;
 
		ABlindMonsterAIController* AIC = Cast<ABlindMonsterAIController>(GetController());
		if (AIC && AIC->GetBlackboardComponent())
		{
			UBlackboardComponent* BB = AIC->GetBlackboardComponent();
			BB->SetValueAsVector("TargetLocation", Player->GetActorLocation());
			BB->SetValueAsBool("IsAlerted", true);
			BB->SetValueAsBool("IsChasing", true);
 
			UE_LOG(LogTemp, Warning, 
				TEXT("Spelaren SEDD på avstånd %.1f cm, vinkel %.1f grader – intensiv chase!"),
				Distance, AngleDeg);
		}
	}
	else
	{
		ABlindMonsterAIController* AIC = Cast<ABlindMonsterAIController>(GetController());
		if (AIC && AIC->GetBlackboardComponent())
		{
			AIC->GetBlackboardComponent()->SetValueAsVector("TargetLocation", Player->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("else were bIsChasing is true"));
		}
	}
}

void ABlindMonsterCharacter::OnHearNoise(APawn* OtherPawn, const FVector& Location, float Volume)
{
		if (OtherPawn != nullptr && OtherPawn != this && OtherPawn->IsPlayerControlled())
		{
			//Dont react to sound if player hide
			APlayerCharacter* Player = Cast<APlayerCharacter>(OtherPawn);
			if (Player)
			{
				HidingComp = Player->FindComponentByClass<UHidingComponent>();
				if (HidingComp && HidingComp->bHiding)
				{
					UE_LOG(LogTemp, Warning, TEXT("Sound is ignored cuz player hid"));
					return; // Cancels before retargeting location
				}
			}
			
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
				UE_LOG(LogTemp, Warning, TEXT("Spelare hördes vid: %s och med ljudstyrkan: %f"), *Location.ToString(), Volume);
			}
		}
}

void ABlindMonsterCharacter::ResetMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		
		ABlindMonsterAIController* AIController = Cast<ABlindMonsterAIController>(GetController());
		if (AIController && AIController->GetBlackboardComponent())
		{
			AIController->GetBlackboardComponent()->SetValueAsBool("IsAlerted", false);
		}
	}
}

