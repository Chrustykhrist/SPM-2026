// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StalkerMonsterCharacter.generated.h"

UENUM(BlueprintType)
enum class EStalkerMonsterCharacterState : uint8
{
	Idle,
	Stalking,
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
	
	class UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EStalkerMonsterCharacterState CurrentState;
	
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class UAudioComponent* AudioComp;
	
	

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
	float StalkReactionTimeBehind = 5.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	float StalkReactionTimePeek = 7.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	float FleeDistance = 600.0f;
	
	
};
