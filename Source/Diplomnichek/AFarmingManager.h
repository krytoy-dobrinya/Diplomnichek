#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FarmingTypes.h"
#include "AFarmingManager.generated.h"

class AGardenCell;

UCLASS()
class DIPLOMNICHEK_API AFarmingManager : public AActor
{
    GENERATED_BODY()

public:
    AFarmingManager();

    UPROPERTY(EditAnywhere, Category = "Farming")
    TSubclassOf<AGardenCell> GardenCellClass;

    // === Grid params ===

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming|Grid")
    float CellSize = 100.0f; // Размер клетки в см

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming|Grid")
    int32 MaxGridWidth = 20; // Максимальный размер сетки в клетках

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming|Grid")
    int32 MaxGridHeight = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming|Grid")
    FVector GridOrigin; // Левый нижний угол сетки (задаётся в редакторе)

    // === DataTable with plants ===

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming|Data")
    UDataTable* CropDataTable;

    // === Massive with cells ===

    UPROPERTY(BlueprintReadOnly, Category = "Farming")
    TArray<AGardenCell*> Cells;

    // === Methods ===

    UFUNCTION(BlueprintCallable, Category = "Farming")
    AGardenCell* TryTillAtLocation(FVector WorldLocation);

    UFUNCTION(BlueprintCallable, Category = "Farming")
    AGardenCell* TryWaterAtLocation(FVector WorldLocation);

    UFUNCTION(BlueprintCallable, Category = "Farming")
    AGardenCell* TryPlantAtLocation(FVector WorldLocation, int32 SeedItemID);

    UFUNCTION(BlueprintCallable, Category = "Farming")
    bool TryHarvestAtLocation(FVector WorldLocation, ACPP_BaseItemClass*& OutItem);

    UFUNCTION(BlueprintCallable, Category = "Farming")
    void UpdateAllCellsAtEndOfDay();

    // Выровнять точку по сетке
    UFUNCTION(BlueprintPure, Category = "Farming")
    FVector SnapToGrid(FVector WorldLocation) const;

    // Найти клетку по позиции
    UFUNCTION(BlueprintPure, Category = "Farming")
    AGardenCell* FindCellAtGrid(int32 GridX, int32 GridY) const;

protected:
    virtual void BeginPlay() override;
};