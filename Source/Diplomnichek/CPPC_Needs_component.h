// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCPP_LightingSourceItemClass.h"
#include "CPPC_Needs_component.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIPLOMNICHEK_API UCPPC_Needs_component : public UActorComponent
{
    GENERATED_BODY()

public:	
    UCPPC_Needs_component();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) override;

    // Энергия персонажа (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
    float Energy = 100.0f;

    // Тепло персонажа (0-100, 0 — замёрз)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
    float Warmth = 100.0f;

    // Активный источник света в руке (nullptr — ничего не держит)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
    AMyCPP_LightingSourceItemClass* ActiveLightItem = nullptr;

    // Для отладки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugMode = true;

private:
    // Таймер для расхода топлива и энергии (раз в секунду)
    float TimeAccumulator = 0.0f;
};
