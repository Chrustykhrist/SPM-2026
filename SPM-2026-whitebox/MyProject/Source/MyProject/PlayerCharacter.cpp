// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "iostream"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	/*
	Timeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
	
	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
	
	Offset = 20.0f;
	*/
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (Subsystem && InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
	
	MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	
	WalkSpeed = MovementComponent->MaxWalkSpeed;
	
	StandingHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CrouchingHeight = MovementComponent->GetCrouchedHalfHeight();
	/*
	if (FCurve && Timeline)
	{
		Timeline->AddInterpFloat(FCurve, InterpFunction, FName("Alpha"));
	}
	*/
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if ((!bRunning || bCrouching) && Stamina < 5)
	{
		Stamina += GetWorld()->GetDeltaSeconds();
	}
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* UEnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UEnhancedInput->BindAction(IAMove, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		UEnhancedInput->BindAction(IAMove, ETriggerEvent::Completed, this, &APlayerCharacter::StopMoving);
		
		UEnhancedInput->BindAction(IALook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		UEnhancedInput->BindAction(IALookMouse, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		
		UEnhancedInput->BindAction(IAJump, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerJump);
		
		UEnhancedInput->BindAction(IACrouch, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerCrouch);
		UEnhancedInput->BindAction(IACrouch, ETriggerEvent::Completed, this, &APlayerCharacter::PlayerUnCrouch);
		
		UEnhancedInput->BindAction(IASprint, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
		UEnhancedInput->BindAction(IASprint, ETriggerEvent::Completed, this, &APlayerCharacter::SlowDown);
	}

}
/*
void APlayerCharacter::TimelineFloatReturn(float Value)
{
	float NewHeight = FMath::Lerp(StandingHeight, CrouchingHeight, Value);
    
	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight);
	
	FVector MeshLocation = GetMesh()->GetRelativeLocation();

	MeshLocation.Z = -NewHeight;
    
	GetMesh()->SetRelativeLocation(MeshLocation);
}
*/
/*
void APlayerCharacter::OnTimelineFinished()
{
	
}
*/
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	bMoving = true;
	AddMovementInput(GetActorForwardVector(), Value.Get<FVector2D>().Y);
	AddMovementInput(GetActorRightVector(), Value.Get<FVector2D>().X);
}

void APlayerCharacter::StopMoving(const FInputActionValue& Value)
{
	bMoving = false;
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<FVector2D>().Y * Sensitivity * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(Value.Get<FVector2D>().X * Sensitivity * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::PlayerJump(const FInputActionValue& Value)
{
	MakeNoise(1.0f, this, GetActorLocation());
	Jump();
	
}

void APlayerCharacter::PlayerCrouch(const FInputActionValue& Value)
{
	bCrouching = true;
	
	MovementComponent->MaxWalkSpeed = CrouchSpeed;
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(StandingHeight, CrouchingHeight, 1.0f));
	
	//Crouch();
	
	//MakeNoise(1.0f, this, GetActorLocation());

}

void APlayerCharacter::PlayerUnCrouch(const FInputActionValue& Value)
{
	bCrouching = false;
	
	MovementComponent->MaxWalkSpeed = WalkSpeed;
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(CrouchingHeight, StandingHeight, 1.0f));
	
	//UnCrouch();
}

void APlayerCharacter::Sprint(const FInputActionValue& Value)
{
	bRunning = true;
	
	if (Stamina > 0 && !bCrouching && bMoving)
	{
		MovementComponent->MaxWalkSpeed = SprintSpeed;
		Stamina -= GetWorld()->GetDeltaSeconds();
	} else if (bCrouching)
	{
		MovementComponent->MaxWalkSpeed = CrouchSpeed;
		bRunning = false;
	} else
	{
		MovementComponent->MaxWalkSpeed = WalkSpeed;
		bRunning = false;
	}
	
}

void APlayerCharacter::SlowDown(const FInputActionValue& Value)
{
	MovementComponent->MaxWalkSpeed *= 0;
	MovementComponent->MaxWalkSpeed = WalkSpeed;
	bRunning = false;
}
