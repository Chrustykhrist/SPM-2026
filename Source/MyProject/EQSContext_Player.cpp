// Fill out your copyright notice in the Description page of Project Settings.


#include "EQSContext_Player.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

void UEQSContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	const AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, PlayerActor);
}
