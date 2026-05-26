// Fill out your copyright notice in the Description page of Project Settings.




#include "KeyPadComponent.h"
#include "CustomPlayerState.h"
#include "PickUp.h"
#include "PlayerCharacter.h"


// Sets default values for this component's properties
UKeyPadComponent::UKeyPadComponent()
{
   // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
   // off to improve performance if you don't need them.
   PrimaryComponentTick.bCanEverTick = true;


   // ...
}




// Called when the game starts
void UKeyPadComponent::BeginPlay()
{
   Super::BeginPlay();


   // int num1 = FMath::RandRange(0, 9);
   // int num2 = FMath::RandRange(0, 9);
   // int num3 = FMath::RandRange(0, 9);
   // int num4 = FMath::RandRange(0, 9);
   //
   // FString SNum1 = FString::FromInt(num1);
   // FString SNum2 = FString::FromInt(num2);
   // FString SNum3 = FString::FromInt(num3);
   // FString SNum4 = FString::FromInt(num4);
   //
   // NeededCode.Add(FName(SNum1));
   // NeededCode.Add(FName(SNum2));
   // NeededCode.Add(FName(SNum3));
   // NeededCode.Add(FName(SNum4));
   //
   // UE_LOG(LogTemp, Warning, TEXT("%s"), *SNum1);
   // UE_LOG(LogTemp, Warning, TEXT("%s"), *SNum2);
   // UE_LOG(LogTemp, Warning, TEXT("%s"), *SNum3);
   // UE_LOG(LogTemp, Warning, TEXT("%s"), *SNum4);
   //
   // // Find all the yaws of the doors
   // for (int i = 0; i < Doors.Num(); i++)
   // {
   //     DoorYaws.Add(Doors[i]->GetActorRotation().Yaw);
   // }
  
   // ...
  
   APlayerController* PC = GetWorld()->GetFirstPlayerController();
   ACustomPlayerState* PS = PC ? PC->GetPlayerState<ACustomPlayerState>() : nullptr;
  
   FName KeypadName = FName(*GetOwner()->GetName());


   if (PS && PS->IsKeypadSolved(KeypadName))
   {
      // Restore saved code
      FString SavedCode = PS->GetKeypadCode(KeypadName);
      for (int i = 0; i < 4 && i < SavedCode.Len(); i++)
      {
         NeededCode.Add(FName(*FString::Chr(SavedCode[i])));
      }
      
      // Snap doors open instantly
      for (int i = 0; i < Doors.Num(); i++)
      {
         DoorYaws.Add(Doors[i]->GetActorRotation().Yaw);
         float TargetYaw = Doors[i]->ActorHasTag("Left") ?
            DoorYaws[i] + 90 : DoorYaws[i] - 90;
         Doors[i]->SetActorRotation(FRotator(0, TargetYaw, 0));
      }
      bTurn = false; // Door already in position, no need to animate
   }
   else
   {
      // Generate new random code as before
      for (int i = 0; i < 4; i++)
      {
         int num = FMath::RandRange(0, 9);
         NeededCode.Add(FName(*FString::FromInt(num)));
         UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::FromInt(num));
      }


      for (int i = 0; i < Doors.Num(); i++)
      {
         DoorYaws.Add(Doors[i]->GetActorRotation().Yaw);
         DoorOffsets.Add(0.0f);
      }
   }
  
}




// Called every frame
void UKeyPadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


   // ...


   // Turn the doors 90 degrees
   if (bTurn)
   {
      for (int i = 0; i < Doors.Num(); i++)
      {
         FRotator TargetRotation = FRotator(0, DoorYaws[i] + DoorOffsets[i], 0);
         Doors[i]->SetActorRotation(FMath::RInterpConstantTo(Doors[i]->GetActorRotation(), TargetRotation, DeltaTime, 30));
      }
   }
  
}


/**
* Adds the number that is pressed to PressedButtons
*
* @param number The pressed number, Tag of the component
*/
void UKeyPadComponent::Pressed(FName number)
{
   PressedButtons.Add(number);
}


/**
* Opens the locked door when the correct numbers are inputted
*/
void UKeyPadComponent::Accepted()
{
   if (PressedButtons.Num() < 4)
   {
      return;
   }


   // Check if the code is correct
   for (int i = 0; i < NeededCode.Num(); i++)
   {
      UE_LOG(LogTemp, Warning, TEXT("%s"), (PressedButtons[i] != NeededCode[i]) ? TEXT("Different") : TEXT("Same"));
              
      if (PressedButtons[i] != NeededCode[i])
      {
         bCorrectInput = false;
      }
   }


   // If correct, allow the door to turn, otherwise do nothing
   if (bCorrectInput)
   {
      //Turn = true;
      
      OpenDoors();
      
      UE_LOG(LogTemp, Display, TEXT("Correct"));
      // Save keypad solved state
      APlayerController* PC = GetWorld()->GetFirstPlayerController();
      if (PC)
      {
         ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();
         if (PS)
         {
            // Build code string to save
            FString CodeString;
            for (const FName& Num : NeededCode)
               CodeString += Num.ToString();
              
            PS->SetKeypadSolved(FName(*GetOwner()->GetName()), CodeString);
            PS->TriggerSaveGame();
         }
      }
     
   }
   else
   {
      ClearPressed();
      UE_LOG(LogTemp, Warning, TEXT("Incorrect"));
      bCorrectInput = true;
   }
}


/**
* Removes all the buttons the user had pressed, called if more than 4 buttons are pressed or the clear button is pressed
*/
void UKeyPadComponent::ClearPressed()
{
   PressedButtons.Empty();
   UE_LOG(LogTemp, Display, TEXT("Clear Pressed"));
}

void UKeyPadComponent::OpenDoors()
{
   APawn* Pawn = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
  
   if (Pawn == nullptr) return;
  
   FVector PawnLocation = Pawn->GetActorLocation();
   UE_LOG(LogTemp, Warning, TEXT("OpenDoors| Before loop"));
   for (int i = 0; i < Doors.Num(); i++)
   {
      FVector DoorForward = FRotator(0, DoorYaws[i], 0).Vector();
      FVector ToDoor = (Doors[i]->GetActorLocation() - PawnLocation).GetSafeNormal();
     
      float Dot = FVector::DotProduct(DoorForward, ToDoor);
      DoorOffsets[i] = (Dot >= 0.0f) ? 90.0f : -90.0f;
      UE_LOG(LogTemp, Warning, TEXT("OpenDoors| Inside loop"));
      // Save that this door was opened
      APlayerController* PC = GetWorld()->GetFirstPlayerController();
      if (PC)
      {
         ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();
         if (PS)
         {
            PS->SetDoorOpened(FName(*Doors[i]->GetName()));
            UE_LOG(LogTemp, Warning, TEXT("OpenDoors| PS SetDoorOpened %s"), *Doors[i]->GetName());
         }
      }
   }
  
   bTurn = true;
}


TArray<FName> UKeyPadComponent::GetNeededCode()
{
   return NeededCode;
}

FString UKeyPadComponent::ShowPressed()
{
   FString Result = "";
   for (const FName& Button : PressedButtons)
   {
      Result += Button.ToString();
   }
   return Result;
}

// Restore the state of the keypad
void UKeyPadComponent::RestoreState(ACustomPlayerState* PS)
{
   if (PS == nullptr) return;
  
   FName KeypadName = FName(*GetOwner()->GetName());
   if (!PS->IsKeypadSolved(KeypadName)) return;


   FString SavedCode = PS->GetKeypadCode(KeypadName);
   NeededCode.Empty();
  
   for (int i = 0; i < 4 && i < SavedCode.Len(); i++)
      NeededCode.Add(FName(*FString::Chr(SavedCode[i])));


   for (int i = 0; i < Doors.Num(); i++)
   {
      float TargetYaw = Doors[i]->ActorHasTag("Left") ?
         DoorYaws[i] + 90 : DoorYaws[i] - 90;
      Doors[i]->SetActorRotation(FRotator(0, TargetYaw, 0));
   }
   bTurn = false;
  
   UE_LOG(LogTemp, Warning, TEXT("RestoreState| Final NeededCode:"));
   for (const FName& Num : NeededCode)
      UE_LOG(LogTemp, Warning, TEXT("  %s"), *Num.ToString());
}
