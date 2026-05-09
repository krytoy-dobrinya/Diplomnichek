#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingData.h"
#include "ABuilding.generated.h"

UCLASS()
class DIPLOMNICHEK_API ABuilding : public AActor
{
    GENERATED_BODY()

public:
    ABuilding();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UBuildingData* BuildingData;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;
};