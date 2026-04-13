// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseItemClass.h"
#include "MyCPP_ToolItemClass.generated.h"


UENUM(BlueprintType)
enum class ECPP_ToolType : uint8
{
    Hoe          UMETA(DisplayName = "Hoe"),
    WateringCan  UMETA(DisplayName = "Watering Can"),
    Axe          UMETA(DisplayName = "Axe"),
    Pickaxe      UMETA(DisplayName = "Pickaxe")
};

UCLASS()
class DIPLOMNICHEK_API AMyCPP_ToolItemClass : public ACPP_BaseItemClass
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float CPP_Energy_cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    ECPP_ToolType CPP_Tool_type;

};
