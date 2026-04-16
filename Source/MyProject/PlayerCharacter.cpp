// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeingPlay"));
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (Subsystem && InputMappingCcntext)
			{
				Subsystem->AddMappingContext(InputMappingCcntext, 0);
			}
		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bRunning && AsthmaLimit < 5)
	{
		AsthmaLimit += GetWorld()->GetDeltaSeconds();
	}
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* UEnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UEnhancedInput->BindAction(IAMove, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		UEnhancedInput->BindAction(IALook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		UEnhancedInput->BindAction(IALookMouse, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		UEnhancedInput->BindAction(IAJump, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerJump);
		UEnhancedInput->BindAction(IACrouch, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerCrouch);
		UEnhancedInput->BindAction(IACrouch, ETriggerEvent::Completed, this, &APlayerCharacter::PlayerUnCrouch);
		UEnhancedInput->BindAction(IASprint, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
		UEnhancedInput->BindAction(IASprint, ETriggerEvent::Completed, this, &APlayerCharacter::SlowDown);
	}

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	AddMovementInput(GetActorForwardVector(), Value.Get<FVector2D>().Y);
	AddMovementInput(GetActorRightVector(), Value.Get<FVector2D>().X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<FVector2D>().Y * Sensitivity * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(Value.Get<FVector2D>().X * Sensitivity * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::PlayerJump(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerJump"));
	MakeNoise(1.0f, this, GetActorLocation());
	Jump();
	
}

void APlayerCharacter::PlayerCrouch(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerCrouch"));
	//MakeNoise(1.0f, this, GetActorLocation());
	Crouch();
}

void APlayerCharacter::PlayerUnCrouch(const FInputActionValue& Value)
{
	UnCrouch();
}

void APlayerCharacter::Sprint(const FInputActionValue& Value)
{
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	bRunning = true;
	
	if (AsthmaLimit > 0) {
		MovementComponent->MaxWalkSpeed = 900;
		AsthmaLimit -= GetWorld()->GetDeltaSeconds();
	} else
	{
		MovementComponent->MaxWalkSpeed = 600;
	}
	
}

void APlayerCharacter::SlowDown(const FInputActionValue& Value)
{
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	MovementComponent->MaxWalkSpeed *= 0;
	MovementComponent->MaxWalkSpeed = 600;
	bRunning = false;
}
