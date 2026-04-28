// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionvalue.h"
#include "PlayerCharacter.generated.h"

class UHidingComponent;

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
	
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputAction* IAMove;
	
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void ShowPauseScreen();

	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void HidePauseScreen();

	void SetHidingComponent(UHidingComponent* NewHidingComponent);

private:
#pragma region InputMethods	
	
	void Move(const FInputActionValue& Value);
	void StopMoving(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void PlayerCrouch(const FInputActionValue& Value);
	void PlayerUnCrouch(const FInputActionValue& Value);
	
	void Sprint(const FInputActionValue& Value);
	void SlowDown(const FInputActionValue& Value);

	void PickUpItem(const FInputActionValue& Value);

	void PauseGame(const FInputActionValue& Value);

	void HideInLocker(const FInputActionValue& Value);
	
#pragma endregion	
	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;
	
	/*
	 *  Booleans used to check what the player is currently doing
	 */
	bool bRunning = false;
	
	bool bCrouching = false;
	
	bool bMoving = false;

	bool bPaused = false;
	
	/*
	 * Variables to change different values on the character and its inputs
	 */
	
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, Category="Input", meta=(Units="cm"))
	float SprintSpeed = 800;
	
	UPROPERTY(EditAnywhere, Category="Input", meta=(Units="cm"))
	float CrouchSpeed = 200;
	
	UPROPERTY(EditAnywhere, Category="Input")
	float Sensitivity = 50;
	
	// How many seconds the player can run
	UPROPERTY(EditAnywhere, Category="Input", BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	float Stamina = 10;

	UPROPERTY()
	float SpeedDecrease;

	UPROPERTY()
	UHidingComponent* HidingComponent;
};
