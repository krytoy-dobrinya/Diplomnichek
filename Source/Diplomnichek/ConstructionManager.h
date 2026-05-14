#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructionManager.generated.h"

class UBuildingData;

UCLASS()
class DIPLOMNICHEK_API AConstructionManager : public AActor
{
    GENERATED_BODY()

public:
    AConstructionManager();

    // Start placing building
    UFUNCTION(BlueprintCallable, Category = "Building")
    void StartPlacing(UBuildingData* Data);

    // Stop placing building
    UFUNCTION(BlueprintCallable, Category = "Building")
    void ConfirmPlacement();

    UFUNCTION(BlueprintCallable, Category = "Building")
    void CancelPlacement();

    // Data of current building
    UPROPERTY(BlueprintReadOnly, Category = "Building")
    UBuildingData* CurrentBuildingData;

    // Building ghost
    UPROPERTY()
    AActor* GhostActor;

    // Materials
    UPROPERTY(EditAnywhere, Category = "Building")
    UMaterialInterface* ValidMaterial;

    UPROPERTY(EditAnywhere, Category = "Building")
    UMaterialInterface* InvalidMaterial;

    UPROPERTY(EditAnywhere, Category = "Building")
    float PlacementGridSize = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Building")
    float GhostHeight = 50.0f;

    // State
    UPROPERTY(BlueprintReadOnly, Category = "Building")
    bool bIsPlacing = false;

    UFUNCTION(BlueprintPure, Category = "Building")
    bool IsPlacing() const { return bIsPlacing; }

    FVector OriginalCameraTransform;
    FRotator OriginalCameraRotation;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    void UpdateGhostPosition();
    bool CanPlaceAtLocation(FVector Location) const;
};