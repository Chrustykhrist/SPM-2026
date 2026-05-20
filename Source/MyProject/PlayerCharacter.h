// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionvalue.h"
#include "PlayerCharacter.generated.h"

#pragma region PREDECLARATION
class UFootstepComponent;
class UInteractionComponent;
class UHidingComponent;
class UCharacterMovementComponent;
class UInputMappingContext;
class UInputAction;
#pragma endregion

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

#pragma region INPUT_ACTIONS	
	/*
	 *  Variables to connect input actions to the code
	 */
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAMove;
	
	// Input to control camera movement with controller
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IALook;
	
	// Input to control camera movement with mouse
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IALookMouse;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IACrouch;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IASprint;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAUse;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAPause;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAHide;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAHoldBreath;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAFlashlight;
#pragma endregion	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region PAUSE_SHOW	
	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void ShowPauseScreen();

	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void HidePauseScreen();
#pragma endregion
	
#pragma region GET/SET	
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetHidingComponent(UHidingComponent* NewHidingComponent);
	
	bool GetMoving() const { return bMoving; }
	
	UFootstepComponent* GetFootstepComponent() { return FootstepComponent; }
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool GetPaused() const { return bPaused; }
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetPaused(bool bNewPaused) { bPaused = bNewPaused; }
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool IsOnPauseStartScreen() const { return bOnPauseStartScreen; }
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetOnPauseStartScreen(bool bNewPauseStart) { bOnPauseStartScreen = bNewPauseStart; }
#pragma endregion
private:
#pragma region INPUT_METHODS	
	
	void Move(const FInputActionValue& Value);
	void StopMoving(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	void ControllerLook(const FInputActionValue& Value);
	
	void PlayerCrouch(const FInputActionValue& Value);
	void PlayerUnCrouch(const FInputActionValue& Value);
	
	void Sprint(const FInputActionValue& Value);
	void SlowDown(const FInputActionValue& Value);

	void PickUpItem(const FInputActionValue& Value);

	void PauseGame(const FInputActionValue& Value);

	void HideInLocker(const FInputActionValue& Value);
	void HoldBreath(const FInputActionValue& Value);
	void ReleaseBreath(const FInputActionValue& Value);
	
	void InteractBegin(const FInputActionValue& Value);
	void InteractHold(const FInputActionValue& Value);
	void InteractEnd(const FInputActionValue& Value);
	
	void UseFlashlight(const FInputActionValue& Value);
#pragma endregion	
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	
	/*
	 *  Booleans used to check what the player is currently doing
	 */
#pragma region BOOLEANS	
	bool bRunning = false;
	
	bool bCrouching = false;
	
	bool bMoving = false;

	bool bPaused = false;
	
	bool bOnPauseStartScreen = true;

	bool bHoldBreath = false;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bHiding = false;
#pragma endregion	
	/*
	 * Variables to change different values on the character and its inputs
	 */

#pragma region VARIABLES	
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, Category="Input")
	float SprintSpeed = 800;
	
	UPROPERTY(EditAnywhere, Category="Input")
	float CrouchSpeed = 200;
	
	UPROPERTY(EditAnywhere, Category="Input")
	float MouseSensitivity = 50;
	
	UPROPERTY(EditAnywhere, Category="Input")
	float ControllerSensitivity = 100;
	
	// How many seconds the player can run
	UPROPERTY(EditAnywhere, Category="Input", BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	float Stamina = 10;

	UPROPERTY(EditAnywhere, Category="Input")
	float RecoveryRate = 0.5;

	UPROPERTY()
	float SpeedDecrease;

	UPROPERTY()
	UHidingComponent* HidingComponent;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	float SprintLoudnessMultiplier = 0.65f;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	float WalkLoudnessMultiplier = 0.35f;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	float CrouchLoudnessMultiplier = 0.15f;
	
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UInteractionComponent> InteractionComponent;
	
	float CurrentTurnDelta = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UFootstepComponent* FootstepComponent;
	
#pragma endregion	
};
