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

    // Начать размещение постройки
    UFUNCTION(BlueprintCallable, Category = "Building")
    void StartPlacing(UBuildingData* Data);

    // Закончить (подтвердить или отменить)
    UFUNCTION(BlueprintCallable, Category = "Building")
    void ConfirmPlacement();

    UFUNCTION(BlueprintCallable, Category = "Building")
    void CancelPlacement();

    // Данные текущей постройки
    UPROPERTY(BlueprintReadOnly, Category = "Building")
    UBuildingData* CurrentBuildingData;

    // Призрак постройки
    UPROPERTY()
    AActor* GhostActor;

    // Материалы
    UPROPERTY(EditAnywhere, Category = "Building")
    UMaterialInterface* ValidMaterial;

    UPROPERTY(EditAnywhere, Category = "Building")
    UMaterialInterface* InvalidMaterial;

    UPROPERTY(EditAnywhere, Category = "Building")
    float PlacementGridSize = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Building")
    float GhostHeight = 50.0f;

    // Состояние
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