// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// called when player begins holding E on this actor
	virtual void OnInteractBegin(APlayerController* PC) = 0;
	// called every frame when holding E
	virtual void OnInteractHold(APlayerController* PC, float Delta) = 0;
	// called when the player releases E
	virtual void OnInteractEnd(APlayerController* PC) = 0;
	
	virtual bool CanInteract() const = 0;
};
