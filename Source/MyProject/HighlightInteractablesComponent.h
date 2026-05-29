#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightInteractablesComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UHighlightInteractablesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHighlightInteractablesComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void EnableHighlight(bool bEnable);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Highlight | Settings")
	float MaxDistance = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Highlight | Settings")
	int32 CustomDepthStencilValue = 1; 

private:
	UPROPERTY()
	TArray<UMeshComponent*> MeshComponents;

	bool bIsHighlighted = false;
	
	UPROPERTY()
	APawn* PlayerPawn;
};
