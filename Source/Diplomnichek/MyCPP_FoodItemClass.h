// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseItemClass.h"
#include "MyCPP_FoodItemClass.generated.h"


UCLASS()
class DIPLOMNICHEK_API AMyCPP_FoodItemClass : public ACPP_BaseItemClass
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float CPP_Energy_restore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float CPP_Warmth_restore;

};
