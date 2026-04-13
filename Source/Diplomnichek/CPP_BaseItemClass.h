// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_BaseItemClass.generated.h"

UENUM(BlueprintType)
enum class ECPP_EquipSlotType : uint8
{
    Hat         UMETA(DisplayName = "Hat"),
    Backpack    UMETA(DisplayName = "Backpack"),
    Toolbar     UMETA(DisplayName = "Toolbar"),
    Simple      UMETA(DisplayName = "Simple")
};


UCLASS()
class DIPLOMNICHEK_API ACPP_BaseItemClass : public AActor
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 CPP_Item_ID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText CPP_Item_name;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText CPP_Item_description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 CPP_Max_stack_size;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 CPP_Row_size;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 CPP_Col_size;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    ECPP_EquipSlotType CPP_Equip_slot_type;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float CPP_Item_value;
    
    ACPP_BaseItemClass();

    virtual void Tick(float DeltaTime) override;
};