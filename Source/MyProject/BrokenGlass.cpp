// Fill out your copyright notice in the Description page of Project Settings.


#include "BrokenGlass.h"

#include "NoiseEmitterComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABrokenGlass::ABrokenGlass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GlassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassMesh"));
	RootComponent = GlassMesh;
	
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	
	NoiseEmitter = CreateDefaultSubobject<UNoiseEmitterComponent>(TEXT("NoiseEmitter"));
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABrokenGlass::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABrokenGlass::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ABrokenGlass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrokenGlass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABrokenGlass::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* Player = Cast<APawn>(OtherActor);
	if (Player && Player->IsPlayerControlled())
	{
		GetWorld()->GetTimerManager().SetTimer(StepTimerHandle, this, &ABrokenGlass::PlayerStep, 0.5f, true);
		//NoiseEmitter->TriggerNoise(Player, 0.8f, GetActorLocation());
	}
}

void ABrokenGlass::PlayerStep()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn && PlayerPawn->GetVelocity().Size() > 0)
	{
		NoiseEmitter->TriggerNoise(PlayerPawn, 0.8f, GetActorLocation());
	}
}

void ABrokenGlass::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* PlayerPawn = Cast<APawn>(OtherActor);
	if (PlayerPawn && PlayerPawn->IsPlayerControlled())
	{
		GetWorld()->GetTimerManager().ClearTimer(StepTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Stoppa ljudet"));
	}
	
}
