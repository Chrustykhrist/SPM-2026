// Fill out your copyright notice in the Description page of Project Settings.




#include "InteractionComponent.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"
// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
   // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
   // off to improve performance if you don't need them.
   PrimaryComponentTick.bCanEverTick = false;


   // ...
}




// Called when the game starts
void UInteractionComponent::BeginPlay()
{
   Super::BeginPlay();


   // ...
  
}




// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


   // ...
}


AActor* UInteractionComponent::FindInteractingActor() const
{
   FVector StartLocation = GetComponentLocation();
   FVector EndLocation = StartLocation + GetForwardVector() * MaxInteractionDistance;
  
   FHitResult ActorHit;
   FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRadius);
  
   // Going to change the TraceChannel to 13 or something but will do that later
   // when i know no one else changes the project setting since there will be annoying conflicts
   bool bHit = GetWorld()->SweepSingleByChannel(ActorHit, StartLocation,
      EndLocation, FQuat::Identity, ECC_Visibility, Sphere);
   
   AActor* HitActor = ActorHit.GetActor();
#if WITH_EDITOR
   //DrawDebugSphere(GetWorld(), EndLocation, InteractionRadius, 12, FColor::Red, false, 2.0f);
#endif 
   if (bHit && HitActor)
   {
      if (HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
      {
         return HitActor;
      }
      
      //UE_LOG(LogTemp, Warning, TEXT("Hit %s but it has no interactable interface"), *HitActor->GetName());
   }
   else
   {
      //UE_LOG(LogTemp, Warning, TEXT("FindInteractingActor did not hit anything."));
   }
  
   return nullptr;
}


void UInteractionComponent::BeginInteract()
{
   
   if (bIsInteracting) return;
   
   AActor* TargetActor = FindInteractingActor();
   
   if (!TargetActor) return;
  
   IInteractable* InteractableActor = Cast<IInteractable>(TargetActor);
   if (InteractableActor && InteractableActor->CanInteract())
   {
      CurrentInteractingActor = TargetActor;
      bIsInteracting = true;
     
      // old code for physical mouse circular data
      // bHasLastMousePos = false;
      // LastMousePos = FVector2D::ZeroVector;
      // bHasLastStickAngle = false;
      // VirtualStickPos = FVector2D::ZeroVector;
     
      InteractableActor->OnInteractBegin(GetWorld()->GetFirstPlayerController());
   }
}


void UInteractionComponent::InteractHeld()
{
   // if (!bIsInteracting || !CurrentInteractingActor) return;
   // //UE_LOG(LogTemp, Warning, TEXT("Interact Held"));
   //
   // // does a proximity check if the player is close enough to the valve
   // // so they cant just hold E and the walk away and turn the valve from anywhere in the level
   // FVector VectorToValveFromPlayer = CurrentInteractingActor->GetActorLocation() - GetComponentLocation();
   // float Distance = VectorToValveFromPlayer.SizeSquared();
   // // dont forget to adjust maxinteractiondistance its at 500 in the editor now
   // if (Distance > FMath::Square(MaxInteractionDistance))
   // {
   //    //UE_LOG(LogTemp, Warning, TEXT("Player to far away from valve"));
   //    EndInteract();
   //    return;
   // }
   //
   // // makes sure that the player is looking at the valve when turning it and dont care about lenght of vector only
   // // direction difference between the angle of valve and player
   // FVector DirectionToValve = VectorToValveFromPlayer.GetSafeNormal();
   // float Dot = FVector::DotProduct(GetForwardVector(), DirectionToValve);
   //
   // if (Dot < AcceptableLookRatio)
   // {
   //    //UE_LOG(LogTemp, Warning, TEXT("Player looked away from valve"));
   //    EndInteract();
   //    return;
   // }
   // IInteractable* InteractableActor = Cast<IInteractable>(CurrentInteractingActor);
   // if (InteractableActor)
   // {
   //    //InteractableActor->OnInteractHold(GetWorld()->GetFirstPlayerController(), Delta);
   //    float CircularDelta = ComputeCircularDelta();
   //    InteractableActor->OnInteractHold(
   //       GetWorld()->GetFirstPlayerController(), CircularDelta);
   // }
   
   if (!bIsInteracting || !CurrentInteractingActor) return;

   FVector VectorToValveFromPlayer = CurrentInteractingActor->GetActorLocation() 
                                   - GetComponentLocation();
   float Distance = VectorToValveFromPlayer.SizeSquared();
   if (Distance > FMath::Square(MaxInteractionDistance))
   {
      EndInteract();
      return;
   }

   FVector DirectionToValve = VectorToValveFromPlayer.GetSafeNormal();
   float Dot = FVector::DotProduct(GetForwardVector(), DirectionToValve);
   if (Dot < AcceptableLookRatio)
   {
      EndInteract();
      return;
   }

   IInteractable* InteractableActor = Cast<IInteractable>(CurrentInteractingActor);
   if (!InteractableActor) return;
   
   RotationDelta = ValveRotationSpeed * GetWorld()->GetDeltaSeconds();
   InteractableActor->OnInteractHold(GetWorld()->GetFirstPlayerController(), RotationDelta);
   
   // old code for circular motion
   // APlayerController* PC = GetWorld()->GetFirstPlayerController();
   // float MouseX, MouseY;
   // float CircularDelta = 0.0f;
   //
   // if (PC && PC->GetMousePosition(MouseX, MouseY))
   // {
   //    // Mouse is available — use screen-space circular delta
   //    CircularDelta = ComputeCircularDelta();
   // }
   // else
   // {
   //    // No mouse (controller) — use right stick circular delta
   //    CircularDelta = ComputeCircularDeltaFromStick(Input);
   // }
   //
   // InteractableActor->OnInteractHold(PC, CircularDelta);
}


void UInteractionComponent::EndInteract()
{
   if (!bIsInteracting || !CurrentInteractingActor) return;
  
   IInteractable* InteractableActor = Cast<IInteractable>(CurrentInteractingActor);
   if (InteractableActor)
   {
      InteractableActor->OnInteractEnd(GetWorld()->GetFirstPlayerController());
   }
  
   CurrentInteractingActor = nullptr;
   bIsInteracting = false;
}

//
// float UInteractionComponent::ComputeCircularDelta()
// {
//    APlayerController* PC = GetWorld()->GetFirstPlayerController();
//    if (!PC || !CurrentInteractingActor) return 0.0f;
//
//
//    float MouseX, MouseY;
//    if (!PC->GetMousePosition(MouseX, MouseY)) return 0.0f;
//    FVector2D CurrentMousePos(MouseX, MouseY);
//
//
//    // project the valves world location to screen space
//    FVector2D ValveScreenPos;
//    if (!PC->ProjectWorldLocationToScreen(
//          CurrentInteractingActor->GetActorLocation(), ValveScreenPos))
//    {
//       return 0.0f;
//    }
//
//
//    // First frame — record position and return no delta
//    if (!bHasLastMousePos)
//    {
//       LastMousePos = CurrentMousePos;
//       bHasLastMousePos = true;
//       return 0.0f;
//    }
//
//
//    FVector2D ToLast = LastMousePos - ValveScreenPos;
//    FVector2D ToCurrent = CurrentMousePos - ValveScreenPos;
//   
//    // ignore the cursor if its too close to center so it doesnt act weird
//    if (ToLast.SizeSquared() < IgnoreCentreThreshold || ToCurrent.SizeSquared() < IgnoreCentreThreshold)
//    {
//       LastMousePos = CurrentMousePos;
//       return 0.0f;
//    }
//
//
//    float AngleLast = FMath::Atan2(ToLast.Y, ToLast.X);
//    float AngleCurrent = FMath::Atan2(ToCurrent.Y, ToCurrent.X);
//
//
//    float Delta = AngleCurrent - AngleLast;
//
//
//    // Wrap to [-PI, PI] so crossing the 180-degree boundary doesn't spike
//    if (Delta > PI) Delta -= 2.0f * PI;
//    if (Delta < -PI) Delta += 2.0f * PI;
//
//
//    LastMousePos = CurrentMousePos;
//
//
//    return FMath::RadiansToDegrees(Delta);
// }
//
// float UInteractionComponent::ComputeCircularDeltaFromStick(FVector2D StickInput)
// {
//    
//    if (StickInput.SizeSquared() < 0.1f)
//    {
//       bHasLastStickAngle = false; // reset so re-engaging doesn't spike
//       return 0.0f;
//    }
//    
//    float CurrentAngle = FMath::Atan2(StickInput.Y, StickInput.X);
//    
//    if (!bHasLastStickAngle)
//    {
//       LastStickAngle = CurrentAngle;
//       bHasLastStickAngle = true;
//       return 0.0f;
//    }
//
//    float Delta = CurrentAngle - LastStickAngle;
//    
//    if (Delta > PI) Delta -= 2.0f * PI;
//    if (Delta < -PI) Delta += 2.0f * PI;
//
//    LastStickAngle = CurrentAngle;
//
//    return FMath::RadiansToDegrees(Delta);
//    
// }
