// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizationWidgetComponent.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UCustomizationWidgetComponent : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FLinearColor GetFlashlightColor() const;
	
	UFUNCTION(BlueprintCallable)
	void SetFlashlightColor(FLinearColor NewColor);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	APlayerCharacter* Player;
};
