// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PickUp.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/GameModeBase.h"

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
	
	// Adding the custom mapping context to the character
	
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
	
	// Getting the movement component. Used mainly for changing movement speed for different scenarios
	MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	
	// Setting the WalkSpeed variable to the current MaxWalkSpeed
	WalkSpeed = MovementComponent->MaxWalkSpeed;
	
	// Setting the speed att which the player moves when crouched
	MovementComponent->MaxWalkSpeedCrouched = CrouchSpeed;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Check if the player has stopped running and/or is crouching, 
	// if true recover the stamina of the player
	if ((!bRunning || bCrouching) && Stamina < 5)
	{
		Stamina += GetWorld()->GetDeltaSeconds();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Connecting all input actions to the designated method
	
	if (UEnhancedInputComponent* UEnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		// Movement
		UEnhancedInput->BindAction(IAMove, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		UEnhancedInput->BindAction(IAMove, ETriggerEvent::Completed, this, &APlayerCharacter::StopMoving);
		
		// Looking
		UEnhancedInput->BindAction(IALook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		UEnhancedInput->BindAction(IALookMouse, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		
		// Crouching
		UEnhancedInput->BindAction(IACrouch, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerCrouch);
		UEnhancedInput->BindAction(IACrouch, ETriggerEvent::Completed, this, &APlayerCharacter::PlayerUnCrouch);
		
		// Sprinting
		UEnhancedInput->BindAction(IASprint, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
		UEnhancedInput->BindAction(IASprint, ETriggerEvent::Completed, this, &APlayerCharacter::SlowDown);

		// Use item / Pick up item
		UEnhancedInput->BindAction(IAUse, ETriggerEvent::Started, this, &APlayerCharacter::PickUpItem);
	}

}
#pragma region MOVE
/**
 *  Moves the player
 */
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	bMoving = true;
	// Forward and backwards movement
	AddMovementInput(GetActorForwardVector(), Value.Get<FVector2D>().Y);
	// Left and right movement
	AddMovementInput(GetActorRightVector(), Value.Get<FVector2D>().X);
}

/**
 *  Changes bMoving to false, see use in Sprint function.
 */
void APlayerCharacter::StopMoving(const FInputActionValue& Value)
{
	bMoving = false;
}
#pragma endregion
/**
 *  Lets the player look around
 */
void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// Moves the camera up and down
	AddControllerPitchInput(Value.Get<FVector2D>().Y * Sensitivity * GetWorld()->GetDeltaSeconds());
	// Moves the camera left and right
	AddControllerYawInput(Value.Get<FVector2D>().X * Sensitivity * GetWorld()->GetDeltaSeconds());
}

#pragma region CROUCH
/**
 *  Makes the player crouch
 *
 *  Camera crouching slowdown/movement is managed in the blueprints rather than the code
 */
void APlayerCharacter::PlayerCrouch(const FInputActionValue& Value)
{
	bCrouching = true;
	
	Crouch();
}

/**
 *  Makes the player uncrouch
 *
 * Camera uncrouch slowdown/movement is managed in the blueprints rather than the code
 */
void APlayerCharacter::PlayerUnCrouch(const FInputActionValue& Value)
{
	bCrouching = false;
	
	UnCrouch();
}
#pragma endregion

#pragma region SPRINT
/**
 *  Makes the player sprint for the "Stamina value" amount of time
 */
void APlayerCharacter::Sprint(const FInputActionValue& Value)
{
	bRunning = true;
	
	// Checks that the player has stamina, isn't crouching and is moving,
	// if true make the player quicker, and lower stamina by one per second until it reaches zero.
	if (Stamina > 0 && !bCrouching && bMoving)
	{
		MovementComponent->MaxWalkSpeed = SprintSpeed;
		Stamina -= GetWorld()->GetDeltaSeconds();
	} else if (bCrouching)
	{
		// If player is crouching make sure they only move as quick as crouching speed 
		// Safety net for the possibility of pressing both sprint and crouch,
		// would otherwise result in walking speed
		MovementComponent->MaxWalkSpeed = CrouchSpeed;
		bRunning = false;
	} else
	{
		// make the player as quick as their walking speed
		MovementComponent->MaxWalkSpeed = WalkSpeed;
		bRunning = false;
	}
	
}

/**
 *  Make the player slow down after they stop sprinting
 */
void APlayerCharacter::SlowDown(const FInputActionValue& Value)
{
	// Makes the player slow down quickly
	MovementComponent->MaxWalkSpeed *= 0;
	// sets the players max speed to walk speed
	MovementComponent->MaxWalkSpeed = WalkSpeed;
	bRunning = false;
}
#pragma endregion

void APlayerCharacter::PickUpItem(const FInputActionValue& Value)
{
	UPickUp* PickUp = Cast<UPickUp>(GetComponentByClass(UPickUp::StaticClass()));

	PickUp->PickUp();
}