// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionvalue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MYPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/*
	 *  Variables to connect input actions to the code
	 */
	
	// Variable to connect all the inputs and define what key is used 
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputMappingContext* InputMappingContext;
	
	// Input to control movement
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputAction* IAMove;
	
	// Input to control camera movement with controller
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IALook;
	
	// Input to control camera movement with mouse
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IALookMouse;
	
	// Input to control crouching
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IACrouch;
	
	// Input to control sprinting
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IASprint;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAUse;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAPause;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
#pragma region InputMethods	
	
	// Function to move the player
	void Move(const FInputActionValue& Value);
	// Function to handle the player stopping
	void StopMoving(const FInputActionValue& Value);

	// Function that handles camera movement
	void Look(const FInputActionValue& Value);
	
	// Function to make the player crouch
	void PlayerCrouch(const FInputActionValue& Value);
	
	// Function to make the player stand up from being crouched
	void PlayerUnCrouch(const FInputActionValue& Value);
	
	// Function to make the player quicker when sprinting
	void Sprint(const FInputActionValue& Value);
	
	// Function to make the player slow down after sprinting
	void SlowDown(const FInputActionValue& Value);

	// Function to pick up items
	void PickUpItem(const FInputActionValue& Value);

	// Function to handle pausing
	void PauseGame(const FInputActionValue& Value);
	
#pragma endregion	
	/*
	 *  Components
	 */
	
	// FTimerHandle TimerHandle;
	
	// The movement component of the player character
	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;
	
	/*
	 *  Booleans used to check what the player is currently doing
	 */
	
	// True if the player is running, false if not
	bool bRunning = false;
	
	// True if the player is crouching, false if not
	bool bCrouching = false;
	
	// True if the player is walking, false if not
	bool bMoving = false;
	
	/*
	 * Variables to change different values on the character and its inputs
	 */
	
	// The speed of the player when walking, is set in the unreal editor
	float WalkSpeed;
	
	// speed that is set when sprinting
	UPROPERTY(EditAnywhere, Category="Input")
	float SprintSpeed = 800;
	
	// Speed that is set when crouching
	UPROPERTY(EditAnywhere, Category="Input")
	float CrouchSpeed = 200;
	
	// Sensitivity of the camera
	UPROPERTY(EditAnywhere, Category="Input")
	float Sensitivity = 50;
	
	// How many seconds the player can run
	UPROPERTY(EditAnywhere, Category="Input", BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	float Stamina = 5;
};
