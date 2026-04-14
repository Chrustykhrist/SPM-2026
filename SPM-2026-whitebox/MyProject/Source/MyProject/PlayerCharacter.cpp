// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	WalkSpeed = MovementComponent->MaxWalkSpeed;
	
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
		UEnhancedInput->BindAction(IALook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		UEnhancedInput->BindAction(IALookMouse, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		UEnhancedInput->BindAction(IAJump, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
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
	Jump();
}

void APlayerCharacter::PlayerCrouch(const FInputActionValue& Value)
{
	bCrouching = true;
	Crouch();
}

void APlayerCharacter::PlayerUnCrouch(const FInputActionValue& Value)
{
	bCrouching = false;
	UnCrouch();
}

void APlayerCharacter::Sprint(const FInputActionValue& Value)
{
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	bRunning = true;
	
	if (Stamina > 0 && !bCrouching) {
		MovementComponent->MaxWalkSpeed = SprintSpeed;
		Stamina -= GetWorld()->GetDeltaSeconds();
	} else
	{
		MovementComponent->MaxWalkSpeed = WalkSpeed;
	}
	
}

void APlayerCharacter::SlowDown(const FInputActionValue& Value)
{
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	MovementComponent->MaxWalkSpeed *= 0;
	MovementComponent->MaxWalkSpeed = WalkSpeed;
	bRunning = false;
}
