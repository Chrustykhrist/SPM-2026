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
	
	//Recharge();
	
	if (Flashlight == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UFlashlightComponent was not found"));
	}
	
	// Use saved duration if available, otherwise full recharge
	if (GI && GI->GetSavedFlashlightDuration() >= 0.0f)
	{
		FlashlightDuration = GI->GetSavedFlashlightDuration();
		GI->SetSavedFlashlightDuration(-1.0f); // Clear after use
	}
	else
	{
		Recharge();
	}
	
	//Flashlight->SetLightColor(GI->GetFlashlightColor());
	Flashlight->SetLightColor(GI ? GI->GetFlashlightColor() : FLinearColor::White);
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
	
	if (GetState() == true && FlashlightDuration > 0.0f && !bForeverFlashlight)
	{
		FlashlightDuration -= DeltaTime;
	}
	
	if (FlashlightDuration <= 0.0f)
	{
		Deteriorate();
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

void UFlashlightComponent::Deteriorate() const
{
	Flashlight->SetIntensity(DeterioratedIntensity);
	Flashlight->SetAttenuationRadius(DeterioratedAttenuation);
}

