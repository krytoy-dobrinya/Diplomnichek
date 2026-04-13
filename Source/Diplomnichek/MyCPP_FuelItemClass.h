// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseItemClass.h"
#include "MyCPP_FuelItemClass.generated.h"


UCLASS()
class DIPLOMNICHEK_API AMyCPP_FuelItemClass : public ACPP_BaseItemClass
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float CPP_Fuel_value;

};
