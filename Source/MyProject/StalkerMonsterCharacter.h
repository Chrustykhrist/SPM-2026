// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StalkerMonsterCharacter.generated.h"

UENUM(BlueprintType)
enum class EStalkerMonsterCharacterState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Stalking UMETA(DisplayName = "Stalking"),
	Peeking,
	Approaching,
	Fleeing,
	Killing
};

UCLASS()

class MYPROJECT_API AStalkerMonsterCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AStalkerMonsterCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComp;
	
	// SenseCongif_Sight to adjust the configs of the sight of the monster
	class UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EStalkerMonsterCharacterState CurrentState;
	
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class UAudioComponent* AudioComp;
	
	// Varaibles for how ofter the monster should check if the player sees it
	float LookInterval = 0.1f;
	float LookTimer = 0.0f;
	
	// Method to check if the player is looking at the monster
	bool CheckIfPlayerIsLooking();
	
	// Pawn variable for the player pawn for refrence to the player
	APawn* PlayerPawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetMonsterState(EStalkerMonsterCharacterState NewState);
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	float IntervalUntilKilling = 5.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	float StalkReactionTimePeek = 7.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	float FleeDistance = 600.0f;
	
	// Adjusts the distance of the trace, how far the player can see the monster, 5000 = 50 meters
	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	float TraceScalar = 5000.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
	float FollowRotationSpeed = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
	float FollowRunSpeed = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
	float AttachDistance = 110.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
	float StalkDistance = 100.0f;
	
	bool bIsAttached = false;
	
	class AStalkerMonsterAIController* StalkerMonsterAIController;
};
