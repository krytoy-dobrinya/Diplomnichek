// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseItemClass.h"
#include "MyCPP_SeedItemClass.generated.h"

/**
 * 
 */
UCLASS()
class DIPLOMNICHEK_API AMyCPP_SeedItemClass : public ACPP_BaseItemClass
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 CPP_Plant_ID;

};
