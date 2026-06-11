// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "CPP_BaseItemClass.generated.h"



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
    int32 CPP_stack_size;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 CPP_Max_stack_size;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float CPP_Item_value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* CPP_Item_image;
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    bool UseItem(AActor* User, FVector TargetLocation);
    virtual bool UseItem_Implementation(AActor* User, FVector TargetLocation);

    UFUNCTION(BlueprintPure)
    virtual bool IsUsableOnFarm() const { return false; }

    ACPP_BaseItemClass();
};