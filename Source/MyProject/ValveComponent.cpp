// Fill out your copyright notice in the Description page of Project Settings.




#include "ValveComponent.h"

#include "CustomPlayerState.h"
#include "TubeActor.h"
#include "HighlightInteractablesComponent.h"
// Sets default values for this component's properties
UValveComponent::UValveComponent()
{
   // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
   // off to improve performance if you don't need them.
   PrimaryComponentTick.bCanEverTick = false;
   
}


// Called when the game starts
void UValveComponent::BeginPlay()
{
   Super::BeginPlay();


   // if (ValveMesh)
   // {
   //    InitialMeshRotation = ValveMesh->GetRelativeRotation();
   // }
   if (ValveMesh)
   {
      InitialMeshRotation = ValveMesh->GetRelativeRotation();
      UE_LOG(LogTemp, Warning, TEXT("ValveComponent BeginPlay| InitialMeshRotation: %s"), 
          *InitialMeshRotation.ToString());
   }
   else
   {
      UE_LOG(LogTemp, Error, TEXT("ValveComponent BeginPlay| ValveMesh is null!"));
   }
  
}


// Called every frame
void UValveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


   // ...
}


void UValveComponent::BeginInteraction()
{
   UE_LOG(LogTemp, Warning, TEXT("BeginInteraction"));
   if (bComplete) return;
   bActive = true;
   UE_LOG(LogTemp, Warning, TEXT("bActive is true"));
}


void UValveComponent::ApplyRotationDelta(float Delta)
{
   if (!bActive || bComplete) return;
   UE_LOG(LogTemp, Warning, TEXT("ApplyRotationDelta"));
   
   CurrentRotation = FMath::Clamp(
      CurrentRotation + Delta * InputSensitivity,
      0.0f, RequiredRotationDegrees);
  
   // rotate the mesh of the valve physically on its own local z axis
   if (ValveMesh)
   {
      UE_LOG(LogTemp, Warning, TEXT("Rotate mesh"));
      float MappedRotateAngle = FMath::GetMappedRangeValueClamped(
            FVector2D(0.0f, RequiredRotationDegrees),
            FVector2D(0.0f, RequiredRotationDegrees),
            CurrentRotation);
     
     
      FQuat SpinDelta = FQuat(FVector::UpVector, FMath::DegreesToRadians(MappedRotateAngle));
      FQuat BaseRotation = FQuat(InitialMeshRotation);
      ValveMesh->SetRelativeRotation(BaseRotation * SpinDelta);
   }
  
   if (CurrentRotation >= RequiredRotationDegrees)
   {
      CompleteValve();
   }
}


void UValveComponent::EndInteraction()
{
   bActive = false;
}


void UValveComponent::CompleteValve()
{
   bComplete = true;
   bActive = false;
  
   if (LinkedTube)
   {
      LinkedTube->Drain();
   }
   if (GetOwner() == nullptr || GetWorld() == nullptr) 
   {
      UE_LOG(LogTemp, Error, TEXT("CompleteValve| Owner or World is null"));
      FindHighlightAndDestroy();
      OnValveCompleted.Broadcast();
      return;
   }
   APlayerController* PC = GetWorld()->GetFirstPlayerController();
   if (PC)
   {
      ACustomPlayerState* PS = PC->GetPlayerState<ACustomPlayerState>();
      if (PS)
      {
         PS->SetValveCompleted(FName(*GetOwner()->GetName()));
         PS->TriggerSaveGame();
      }
      else
      {
         UE_LOG(LogTemp, Error, TEXT("CompleteValve| PlayerState is null"));
      }
   }
   else
   {
      UE_LOG(LogTemp, Error, TEXT("CompleteValve| PlayerController is null"));
   }
   
   FindHighlightAndDestroy();
   
   OnValveCompleted.Broadcast();
}

void UValveComponent::RestoreState(ACustomPlayerState* PS)
{
   if (PS == nullptr) return;
   if (!PS->IsValveCompleted(FName(*GetOwner()->GetName()))) return;
   
   bComplete = true;
   bActive = false;
   
   if (ValveMesh)
   {
      if (InitialMeshRotation.IsZero())
      {
         InitialMeshRotation = ValveMesh->GetRelativeRotation();
      }
      FQuat SpinDelta = FQuat(FVector::UpVector, 
          FMath::DegreesToRadians(RequiredRotationDegrees));
      FQuat BaseRotation = FQuat(InitialMeshRotation);
      ValveMesh->SetRelativeRotation(BaseRotation * SpinDelta);
   }
   
   if (LinkedTube)
   {
      LinkedTube->Destroy();
   }

   FindHighlightAndDestroy();
}

void UValveComponent::FindHighlightAndDestroy()
{
   HighlightedComp = 
       GetOwner()->FindComponentByClass<UHighlightInteractablesComponent>();
   if (HighlightedComp)
   {
      HighlightedComp->EnableHighlight(false);
      HighlightedComp->SetActive(false);
      HighlightedComp->DestroyComponent();
   }
}
