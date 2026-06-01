// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CustomizationComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UCustomizationComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	FLinearColor GetFlashlightColor() const { return FlashlightColor; }
	
	UFUNCTION(BlueprintCallable)
	void SetFlashlightColor(FLinearColor NewColor) { FlashlightColor = NewColor; }
	
	UPROPERTY()
	FLinearColor FlashlightColor = FLinearColor::White;
	
};
