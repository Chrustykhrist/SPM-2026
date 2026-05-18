#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightInteractablesComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UHighlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHighlightComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Highlight | Settings")
	float MaxDistance = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Highlight | Settings")
	int32 CustomDepthStencilValue = 1; 

private:
	void EnableHighlight(bool bEnable);

	UPROPERTY()
	TArray<UMeshComponent*> MeshComponents;

	bool bIsHighlighted = false;
	
	UPROPERTY()
	APawn* PlayerPawn;
};
