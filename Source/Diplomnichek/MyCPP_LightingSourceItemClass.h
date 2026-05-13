// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseItemClass.h"
#include "MyCPP_LightingSourceItemClass.generated.h"



UCLASS()
class DIPLOMNICHEK_API AMyCPP_LightingSourceItemClass : public ACPP_BaseItemClass
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float CPP_Max_fuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float CPP_Current_fuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool CPP_isDisposable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool CPP_isLit;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Light Source")
    void ifLit();
};
