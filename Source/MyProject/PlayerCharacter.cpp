// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "CustomPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "FlashlightComponent.h"
#include "PickUp.h"
#include "HidingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "InteractionComponent.h"
#include "FootstepComponent.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));
	
	FootstepComponent = CreateDefaultSubobject<UFootstepComponent>(TEXT("FootstepComponent"));
	// InteractionComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

	SpeedDecrease = 250/Stamina;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Check if the player has stopped running and/or is crouching, 
	// if true recover the stamina of the player
	if (!bRunning && !bHoldBreath && Stamina <= 10)
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
		UEnhancedInput->BindAction(IALook, ETriggerEvent::Triggered, this, &APlayerCharacter::ControllerLook);
		UEnhancedInput->BindAction(IALookMouse, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		
		// Crouching
		UEnhancedInput->BindAction(IACrouch, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerCrouch);
		UEnhancedInput->BindAction(IACrouch, ETriggerEvent::Completed, this, &APlayerCharacter::PlayerUnCrouch);
		
		// Sprinting
		UEnhancedInput->BindAction(IASprint, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
		UEnhancedInput->BindAction(IASprint, ETriggerEvent::Completed, this, &APlayerCharacter::SlowDown);

		// Use item / Pick up item
		UEnhancedInput->BindAction(IAUse, ETriggerEvent::Started, this, &APlayerCharacter::PickUpItem);

		// Pause
		UEnhancedInput->BindAction(IAPause, ETriggerEvent::Started, this, &APlayerCharacter::PauseGame);

		// Hide
		UEnhancedInput->BindAction(IAHide, ETriggerEvent::Started, this, &APlayerCharacter::HideInLocker);

		// Hold breath
		UEnhancedInput->BindAction(IAHoldBreath, ETriggerEvent::Triggered, this, &APlayerCharacter::HoldBreath);
		UEnhancedInput->BindAction(IAHoldBreath, ETriggerEvent::Completed, this, &APlayerCharacter::ReleaseBreath);
		
		// Interaction valves
		UEnhancedInput->BindAction(IAUse, ETriggerEvent::Started, this, &APlayerCharacter::InteractBegin);
		UEnhancedInput->BindAction(IAUse, ETriggerEvent::Completed, this, &APlayerCharacter::InteractEnd);
		UEnhancedInput->BindAction(IALookMouse, ETriggerEvent::Triggered, this, &APlayerCharacter::InteractHold);
		UEnhancedInput->BindAction(IALook, ETriggerEvent::Triggered, this, &APlayerCharacter::InteractHold);
		
		// Flashlight
		UEnhancedInput->BindAction(IAFlashlight, ETriggerEvent::Started, this, &APlayerCharacter::UseFlashlight);
		UEnhancedInput->BindAction(IAUseItem, ETriggerEvent::Started, this, &APlayerCharacter::UseItem);
	}

}
#pragma region MOVE

/**
 *  Moves the player
 */
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!bMoving && GetCapsuleComponent()->GetScaledCapsuleHalfHeight() != 40)
	{
		FootstepComponent->SetMovementState(EMovementState::Walking);
	}
	
	bMoving = true;
	// Forward and backwards movement
	AddMovementInput(GetActorForwardVector(), Value.Get<FVector2D>().Y);
	// Left and right movement
	AddMovementInput(GetActorRightVector(), Value.Get<FVector2D>().X);
	
	if (!bCrouching) MakeNoise(WalkLoudnessMultiplier, this, GetActorLocation());
	
	FootstepComponent->SetIsMoving(true);
}

/**
 *  Changes bMoving to false, see use in Sprint function.
 */
void APlayerCharacter::StopMoving(const FInputActionValue& Value)
{
	bMoving = false;
	FootstepComponent->SetIsMoving(false);
}
#pragma endregion

#pragma region LOOK
/**
 *  Lets the player look around, for keyboard and mouse
 */
void APlayerCharacter::Look(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<FVector2D>().Y * MouseSensitivity * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(Value.Get<FVector2D>().X * MouseSensitivity * GetWorld()->GetDeltaSeconds());
}

/**
 * Lets the player look around, for controller
 */
void APlayerCharacter::ControllerLook(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<FVector2D>().Y * ControllerSensitivity * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(Value.Get<FVector2D>().X * ControllerSensitivity * GetWorld()->GetDeltaSeconds());
}
#pragma endregion

#pragma region CROUCH
/**
 *  Makes the player crouch
 *
 *  Camera crouching slowdown/movement is managed in the blueprints rather than the code
 */
void APlayerCharacter::PlayerCrouch(const FInputActionValue& Value)
{
	//If hiding ignore crouch inputs
	if (bHiding)
	{
		return;
	}
	
	if (!bCrouching)
	{
		bCrouching = true;
		FootstepComponent->SetMovementState(EMovementState::Sneaking);
		Crouch();
	}
	
}

/**
 *  Makes the player uncrouch
 *
 * Camera uncrouch slowdown/movement is managed in the blueprints rather than the code
 */
void APlayerCharacter::PlayerUnCrouch(const FInputActionValue& Value)
{
	if (bHiding)
	{
		return;
	}
	bCrouching = false;
	UnCrouch();
	
	//GetCharacterMovement()->bWantsToCrouch = false;
	
	if (GetCharacterMovement()->bWantsToCrouch == false || GetCapsuleComponent()->GetScaledCapsuleHalfHeight() != 40)
	{
		UE_LOG(LogTemp, Warning, TEXT("Here"));
		FootstepComponent->SetMovementState(EMovementState::Walking);
	}
}
#pragma endregion

#pragma region SPRINT

/**
 *  Makes the player sprint for the "Stamina value" amount of time
 */
void APlayerCharacter::Sprint(const FInputActionValue& Value)
{
	bRunning = true;

	// Slows the player down depending on different conditions
	if (Stamina > 0 && !bCrouching && bMoving)
	{
		if (FootstepComponent->GetCurrentMovementState() != EMovementState::Sprinting && GetCapsuleComponent()->GetScaledCapsuleHalfHeight() != 40)
		{
			FootstepComponent->SetMovementState(EMovementState::Sprinting);
		}
		MakeNoise(SprintLoudnessMultiplier, this, GetActorLocation());
		MovementComponent->MaxWalkSpeed = WalkSpeed + SpeedDecrease * Stamina;
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

/**
 *  Make the player slow down to walkspeed after they stop sprinting
 */
void APlayerCharacter::SlowDown(const FInputActionValue& Value)
{
	MovementComponent->MaxWalkSpeed *= 0;
	MovementComponent->MaxWalkSpeed = WalkSpeed;
	bRunning = false;
	if (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() != 40)
	{
		FootstepComponent->SetMovementState(EMovementState::Walking);
	}
}
#pragma endregion

/**
 * Makes the player pick up the item they are looking at
 */
void APlayerCharacter::PickUpItem(const FInputActionValue& Value)
{
	UPickUp* PickUp = Cast<UPickUp>(GetComponentByClass(UPickUp::StaticClass()));

	PickUp->PickUp();
}

/**
 * Makes the game stop and shows the pause screen 
 */
void APlayerCharacter::PauseGame(const FInputActionValue& Value)
{
	if (!bOnPauseStartScreen)
	{
		return;
	}
	
	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	
	if (bPaused)
	{
		HidePauseScreen();

		bPaused = false;

		PC->SetPause(bPaused);
		
		PC->bShowMouseCursor = false;

		FInputModeGameOnly inputMode;

		PC->SetInputMode(inputMode);
	}
	else
	{
		ShowPauseScreen();

		bPaused = true;
		
		bOnPauseStartScreen = true;
		
		PC->SetPause(bPaused);

		PC->bShowMouseCursor = true;

		FInputModeGameAndUI inputMode;
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		inputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(inputMode);
	}
}

#pragma region HIDING

/**
 * Hides the player in the locker that they are at
 */
void APlayerCharacter::HideInLocker(const FInputActionValue& Value)
{
	if (HidingComponent == nullptr)
	{
		return;
	}
	
	if (bHiding)
	{
		HidingComponent->GetOut();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UnCrouch();
		bHiding = false;
		FootstepComponent->SetIsMoving(true);
	}
	else
	{
		FootstepComponent->SetIsMoving(false);
		bHiding = true;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HidingComponent->Hide();
	}
}

/**
 * Lowers stamina when holding space
 */
void APlayerCharacter::HoldBreath(const FInputActionValue& Value)
{
	if (HidingComponent == nullptr)
	{
		return;
	}
	
	if (HidingComponent->bHiding)
	{
		if (Stamina > 0)
		{
			Stamina -= GetWorld()->GetDeltaSeconds()/2;
		}
		bHoldBreath = true;
		UE_LOG(LogTemp, Display, TEXT("%f"), Stamina);
	}
}

/**
 * Lets the player recover stamina
 */
void APlayerCharacter::ReleaseBreath(const FInputActionValue& Value)
{
	bHoldBreath = false;
}

/**
 * Sets the component the player is hiding in
 */
void APlayerCharacter::SetHidingComponent(UHidingComponent* NewHidingComponent)
{
	HidingComponent = NewHidingComponent;
}

#pragma endregion

#pragma region INTERACTING

void APlayerCharacter::InteractBegin(const FInputActionValue& Value)
{
	UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(GetComponentByClass(UInteractionComponent::StaticClass()));
	if (InteractionComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("InteractBegin"));
		InteractionComponent->BeginInteract();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Else Begin, Component: %s"), InteractionComponent ? *InteractionComponent->GetName() : TEXT("NULL"));
	}
	
}

void APlayerCharacter::InteractHold(const FInputActionValue& Value)
{
	UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(GetComponentByClass(UInteractionComponent::StaticClass()));;
	if (InteractionComponent && InteractionComponent->bIsInteracting)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hold"));
		InteractionComponent->InteractHeld(Value.Get<FVector2D>().X);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("else hold"));
		Look(Value);
	}
}

void APlayerCharacter::InteractEnd(const FInputActionValue& Value)
{
	UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(GetComponentByClass(UInteractionComponent::StaticClass()));;
	if (InteractionComponent && InteractionComponent->bIsInteracting)
	{
		//UE_LOG(LogTemp, Warning, TEXT("End"));
		InteractionComponent->EndInteract();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Else End"));
		// PickUpItem(Value);
	}
}

#pragma endregion	

#pragma region ITEM_USE
void APlayerCharacter::UseFlashlight(const FInputActionValue& Value)
{
	UFlashlightComponent* FL = Cast<UFlashlightComponent>(GetComponentByClass(UFlashlightComponent::StaticClass()));
	
	if (FL == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Flashlight Component"));
		return;
	}
	
	ACustomPlayerState* PS = Cast<ACustomPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	
	if (PS == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Player State Found"));
		return;
	}
	
	if (PS->GetCollectedItems()[FName("Flashlight")] < 1)
	{
		return;
	}
	
	if (FL->GetState())
	{
		FL->TurnOff();
	} else
	{
		FL->TurnOn();
	}
}

void APlayerCharacter::UseItem(const FInputActionValue& Value)
{
	UFlashlightComponent* FL = Cast<UFlashlightComponent>(GetComponentByClass(UFlashlightComponent::StaticClass()));
	
	ACustomPlayerState* PS = Cast<ACustomPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	
	if (PS == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Player State Found"));
		return;
	}
	
	if (FL == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Flashlight Component"));
		return;
	}
	
	if (PS->GetCollectedItems()[FName("Flashlight")] < 1)
	{
		return;
	}
	
	if (SelectedItem == 0)
	{
		return;
	}
	
	if (SelectedItem == 2)
	{
		if (PS->GetCollectedItems()[FName("Battery")] >= 1)
		{
			FL->Recharge();
			PS->GetCollectedItems()[FName("Battery")]--;
		}
	} else if (SelectedItem == 1)
	{
		if (PS->GetCollectedItems()[FName("Medicine")] >= 1)
		{
			PS->GetCollectedItems()[FName("Medicine")]--;
		}
	}
}

void APlayerCharacter::SwitchToFirstItem(const FInputActionValue& Value)
{
	if (SelectedItem == 0 || SelectedItem == 2)
	{
		SelectedItem = 1;
	} else if (SelectedItem == 1)
	{
		SelectedItem = 0;
	}
}

void APlayerCharacter::SwitchToSecondItem(const FInputActionValue& Value)
{
	if (SelectedItem == 0 || SelectedItem == 1)
	{
		SelectedItem = 2;
	} else if (SelectedItem == 2)
	{
		SelectedItem = 0;
	}
}

#pragma endregion
