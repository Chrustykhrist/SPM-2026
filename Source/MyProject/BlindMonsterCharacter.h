// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlindMonsterCharacter.generated.h"

class UHidingComponent;

UCLASS()
class MYPROJECT_API ABlindMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlindMonsterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Death")
	class UBoxComponent* DeathBoxTrigger;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	float SightDistance = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	float SightAngle = 35.0f;
	
	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensingComponent;
	
	void CheckLineOfSight();
	
	UFUNCTION()
	void OnHearNoise(APawn* OtherPawn, const FVector& Location, float Volume);
	
	void ResetMovement();
	
private:
	bool bIsChasing = false;
	
	float CheckSightInterval = 1.0f;
	
	float CheckTimer = 0.f;
	
	UHidingComponent* HidingComp;
};
