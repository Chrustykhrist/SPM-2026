// Fill out your copyright notice in the Description page of Project Settings.




#include "ValveComponent.h"
#include "TubeActor.h"
#include "HighlightInteractablesComponent.h"
// Sets default values for this component's properties
UValveComponent::UValveComponent()
{
   // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
   // off to improve performance if you don't need them.
   PrimaryComponentTick.bCanEverTick = false;
   //ComponentTags.Add(FName("ValveComponent"));

   // ...
}


// Called when the game starts
void UValveComponent::BeginPlay()
{
   Super::BeginPlay();


   if (ValveMesh)
   {
      InitialMeshRotation = ValveMesh->GetRelativeRotation();
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
   // CurrentRotation = FMath::Clamp(CurrentRotation + Delta * InputSensitivity *
   //     GetWorld()->GetDeltaSeconds(), 0.0f, RequiredRotationDegrees);
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
      //ValveMesh->SetRelativeRotation(FRotator(0.0f, MappedRotateAngle, 0.0f));
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
   //UHighlightInteractablesComponent* HighlightedComp = GetOwner()->GetComponentByClass(UHighlightInteractablesComponent::StaticClass());
   UHighlightInteractablesComponent* HighlightedComp = GetOwner()->FindComponentByClass<UHighlightInteractablesComponent>();
   if (HighlightedComp)
   {
      HighlightedComp->EnableHighlight(false);
      HighlightedComp->SetActive(false);
      HighlightedComp->DestroyComponent();
   }
   
   OnValveCompleted.Broadcast();
}
