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
	
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputMappingContext* InputMappingCcntext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputAction* IAMove;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAJump;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IALook;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IALookMouse;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IACrouch;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IASprint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	/*
	 *  Funktioner 
	 */
	void Move(const FInputActionValue& Value);
	void StopMoving(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	void PlayerJump(const FInputActionValue& Value);
	
	void PlayerCrouch(const FInputActionValue& Value);
	void PlayerUnCrouch(const FInputActionValue& Value);
	
	void Sprint(const FInputActionValue& Value);
	void SlowDown(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, Category="Input")
	float Sensitivity = 50;
	
	UPROPERTY(EditAnywhere, Category="Input", BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	float Stamina = 5;
	
	FTimerHandle TimerHandle;
	
	class UCharacterMovementComponent* MovementComponent;
	
	class UCapsuleComponent* CapsuleComponent;
	
	bool bRunning = false;
	bool bCrouching = false;
	bool bMoving = false;
	
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, Category="Input")
	float SprintSpeed = 800;
	
	UPROPERTY(EditAnywhere, Category="Input")
	float CrouchSpeed = 200;

	FVector StandingVector;
	
	UPROPERTY(EditAnywhere, Category="Input")
	FVector CrouchingVector = FVector(0.0f, 88.0f, 0.0f);
};
