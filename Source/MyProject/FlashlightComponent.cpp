// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightComponent.h"

#include "CustomGameInstance.h"

// Sets default values for this component's properties
UFlashlightComponent::UFlashlightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	UCustomGameInstance* GI = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
	
	Flashlight = Cast<USpotLightComponent>(GetOwner()->GetComponentByClass(USpotLightComponent::StaticClass()));
	
	Recharge();
	
	if (Flashlight == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UFlashlightComponent was not found"));
	}
	
	Flashlight->SetLightColor(GI->GetFlashlightColor());
	
	Flashlight->SetActive(false);
	Flashlight->SetVisibility(false);
}


// Called every frame
void UFlashlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (FlashlightDuration <= 0.0f)
	{
		TurnOff();
	}*/
	
	if (GetState() == true && FlashlightDuration > 0.0f)
	{
		FlashlightDuration -= DeltaTime;
	}
	
	// ...
}

void UFlashlightComponent::TurnOn()
{
	Flashlight->SetActive(true);
	Flashlight->SetVisibility(true);
	SetState(true);

}

void UFlashlightComponent::TurnOff()
{
	Flashlight->SetActive(false);
	Flashlight->SetVisibility(false);
	SetState(false);
}

void UFlashlightComponent::Recharge()
{
	FlashlightDuration = MaxFlashlightDuration;
	Flashlight->SetAttenuationRadius(MaxAttenuation);
	Flashlight->SetIntensity(MaxIntensity);
}

