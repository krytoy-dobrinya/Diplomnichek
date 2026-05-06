#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseItemClass.h"
#include "FarmingTypes.generated.h"

// Состояние клетки
UENUM(BlueprintType)
enum class ECellState : uint8
{
    Tilled_Dry          UMETA(DisplayName = "Tilled Dry"),
    Tilled_Watered      UMETA(DisplayName = "Tilled Watered"),
    Planted_Dry         UMETA(DisplayName = "Planted Dry"),
    Planted_Watered     UMETA(DisplayName = "Planted Watered")
};

// Стадия роста
UENUM(BlueprintType)
enum class EGrowthStage : uint8
{
    Seed    UMETA(DisplayName = "Seed"),
    Sprout  UMETA(DisplayName = "Sprout"),
    Final   UMETA(DisplayName = "Final")
};

// Данные о культуре (для DataTable)
USTRUCT(BlueprintType)
struct FCropData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    FText CropName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    int32 SeedItemID = -1;

    // Дни на каждую стадию
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    int32 DaysForSeed = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    int32 DaysForSprout = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    TSubclassOf<ACPP_BaseItemClass> HarvestItemClass; // Blueprint предмета урожая
};