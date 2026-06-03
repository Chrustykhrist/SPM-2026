// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlindMonsterCharacter.generated.h"

class APatrolRoute;
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
	
	UPROPERTY(EditAnywhere, Category = "Active")
	bool bIsActive = true;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
#pragma region Patrol	
	
	// What route should the monster follow as standard
	UPROPERTY(EditInstanceOnly, Category = "Patrol")
	TArray<APatrolRoute*> PatrolRoutes;
	
	// index for current waypoint
	UPROPERTY(BlueprintReadOnly, Category = "Patrol")
	int CurrentWaypointIndex = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Patrol")
	float RouteChangedThreshold = 300.0f;
	
	// Gets the closest route to the player that the monster should use
	void SelectClosestRouteToPlayer();
	
	// gets the next waypoint in line
	AActor* GetNextWaypoint();
	
#pragma endregion	
	
#pragma region SightAttributes	
	
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	float SightDistanceNotChase = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	float SightDistanceChase = 400.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	float SightAngle = 35.0f;
	
#pragma endregion	
	// UPROPERTY(EditDefaultsOnly, Category = "Sight")
	// float DistanceInfront = 200.0f;
	//
	// // how many seconds in the future should the monster predict
	// UPROPERTY(EditDefaultsOnly, Category = "Sight")
	// float PredictionTime = 1.0f;
	
	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensingComponent;
	
	void CheckLineOfSight();
	
	UFUNCTION()
	void OnHearNoise(APawn* OtherPawn, const FVector& Location, float Volume);
	
	void ResetMovement();
	
	UFUNCTION()
	void Respawn();
	
private:
	
	bool CheckIfHiding();
	
	bool CheckIfOutOfDistance();
	
	bool CheckIfOutOfSight();
	
	UPROPERTY()
	APatrolRoute* ActivePatrolRoute = nullptr;
	
	bool bIsChasing = false;
	
	float CheckSightInterval = 0.2f;
	
	float CheckTimer = 0.f;
	
	float Distance;
	
	float AngleDeg;
	
	float BestDistanceSq = FLT_MAX;
	
	float SightDistance = SightDistanceNotChase;
	
	UHidingComponent* HidingComp;
	
	class APlayerCharacter* Player;
	
	FVector PlayerVelocity;
	
	FVector ToPlayer;
	
	FTransform SpawnLocation;
};
