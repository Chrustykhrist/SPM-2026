// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizationWidgetComponent.h"

#include "CustomGameInstance.h"
#include "FlashlightComponent.h"
#include "PlayerCharacter.h"

FLinearColor UCustomizationWidgetComponent::GetFlashlightColor() const
{
	UCustomGameInstance* GI = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
	
	return GI->GetFlashlightColor();
}

void UCustomizationWidgetComponent::SetFlashlightColor(FLinearColor NewColor)
{
	UCustomGameInstance* GI = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
	UFlashlightComponent* FL = Cast<UFlashlightComponent>(Player->GetComponentByClass(UFlashlightComponent::StaticClass()));
	
	GI->SetFlashlightColor(NewColor);
	FL->SetFlashlightColor(NewColor);
}
