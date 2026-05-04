// Fill out your copyright notice in the Description page of Project Settings.


#include "ValveActor.h"
#include "ValveComponent.h"
// Sets default values
AValveActor::AValveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	ValveBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ValveBase"));
	RootComponent = ValveBaseMesh;
	
	ValveWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ValveWheel"));
	ValveWheelMesh->SetupAttachment(ValveBaseMesh);
	
	ValveComponent = CreateDefaultSubobject<UValveComponent>(TEXT("ValveComponent"));
	ValveComponent->ValveMesh = ValveBaseMesh;
}

// Called when the game starts or when spawned
void AValveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AValveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AValveActor::OnInteractBegin(APlayerController* PC)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin interact Valve Actor"));
	ValveComponent->BeginInteraction();
}

void AValveActor::OnInteractHold(APlayerController* PC, float Delta)
{
	UE_LOG(LogTemp, Warning, TEXT("Hold Valve Actor"));
	ValveComponent->ApplyRotationDelta(Delta);
}

void AValveActor::OnInteractEnd(APlayerController* PC)
{
	UE_LOG(LogTemp, Warning, TEXT("End Valve Actor"));
	ValveComponent->EndInteraction();
}

bool AValveActor::CanInteract() const
{
	return !ValveComponent->IsComplete();
}
