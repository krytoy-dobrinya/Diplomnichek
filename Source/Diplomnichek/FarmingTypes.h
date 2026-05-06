#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseItemClass.h"
#include "FarmingTypes.generated.h"

// Cell state
UENUM(BlueprintType)
enum class ECellState : uint8
{
    Tilled_Dry          UMETA(DisplayName = "Tilled Dry"),
    Tilled_Watered      UMETA(DisplayName = "Tilled Watered"),
    Planted_Dry         UMETA(DisplayName = "Planted Dry"),
    Planted_Watered     UMETA(DisplayName = "Planted Watered")
};

// Growth stage
UENUM(BlueprintType)
enum class EGrowthStage : uint8
{
    Seed    UMETA(DisplayName = "Seed"),
    Sprout  UMETA(DisplayName = "Sprout"),
    Final   UMETA(DisplayName = "Final")
};

// Plant data (for DataTable)
USTRUCT(BlueprintType)
struct FCropData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    FText CropName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    int32 SeedItemID = -1;

    // Days for every stage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    int32 DaysForSeed = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    int32 DaysForSprout = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming")
    TSubclassOf<ACPP_BaseItemClass> HarvestItemClass; // Blueprint for harvest item

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming|Visual")
    UStaticMesh* SeedMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming|Visual")
    UStaticMesh* SproutMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Farming|Visual")
    UStaticMesh* FinalMesh;
};