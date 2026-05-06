#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FarmingTypes.h"
#include "AGardenCell.generated.h"

UCLASS()
class DIPLOMNICHEK_API AGardenCell : public AActor
{
    GENERATED_BODY()

public:
    AGardenCell();

    // === Variables ===

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    ECellState CurrentState = ECellState::Tilled_Dry;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    int32 PlantedCropID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    EGrowthStage GrowthStage = EGrowthStage::Seed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    int32 DaysWateredInStage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    bool bWateredToday = false;

    // Coordinates on Grid
    UPROPERTY(BlueprintReadOnly, Category = "Farming")
    int32 GridX = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Farming")
    int32 GridY = 0;

    // Meshes
    UPROPERTY(BlueprintReadOnly, Category = "Farming")
    UStaticMesh* SeedMesh;

    UPROPERTY(BlueprintReadOnly, Category = "Farming")
    UStaticMesh* SproutMesh;

    UPROPERTY(BlueprintReadOnly, Category = "Farming")
    UStaticMesh* FinalMesh;

    // === Components ===

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* PlantMeshComponent; // Меш растения поверх грядки


    // === Methods ===

    UFUNCTION(BlueprintCallable, Category = "Farming")
    bool Till();

    UFUNCTION(BlueprintCallable, Category = "Farming")
    bool Water();

    UFUNCTION(BlueprintCallable, Category = "Farming")
    bool PlantSeed(int32 SeedItemID, int32 DaysForSeed, int32 DaysForSprout, UDataTable* CropDataTable);

    UFUNCTION(BlueprintCallable, Category = "Farming")
    bool Harvest(ACPP_BaseItemClass*& OutItem, UDataTable* CropDataTable);

    UFUNCTION(BlueprintCallable, Category = "Farming")
    void UpdateGrowthAtEndOfDay(UDataTable* CropDataTable);

    // Visual update (called from Blueprint or C++)
    UFUNCTION(BlueprintNativeEvent, Category = "Farming")
    void UpdateVisuals();
    virtual void UpdateVisuals_Implementation();

    // === Events for Blueprint ===
    UFUNCTION(BlueprintImplementableEvent, Category = "Farming")
    void OnStateChanged(ECellState NewState);

protected:
    virtual void BeginPlay() override;

private:
    int32 DaysForSeedStage = 2;
    int32 DaysForSproutStage = 3;
};