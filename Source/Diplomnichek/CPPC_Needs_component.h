// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCPP_LightingSourceItemClass.h"
#include "CPPC_Needs_component.generated.h"

class MyCPP_LightingSourceItemClass;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIPLOMNICHEK_API UCPPC_Needs_component : public UActorComponent
{
	GENERATED_BODY()

public:	
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) override;


    // Энергия персонажа (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
    float Energy = 100.0f;

    // Тепло персонажа (0-100, 0 — замёрз)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
    float Warmth = 100.0f;

    // Активный источник света в руке (nullptr — ничего не держит)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Needs")
    TWeakObjectPtr<AMyCPP_LightingSourceItemClass> ActiveLightItem;

};
