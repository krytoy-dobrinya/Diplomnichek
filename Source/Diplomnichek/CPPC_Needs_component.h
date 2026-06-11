// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCPP_LightingSourceItemClass.h"
#include "CPPC_Needs_component.generated.h"

class AMyCPP_LightingSourceItemClass;

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
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Needs")
    TWeakObjectPtr<AMyCPP_LightingSourceItemClass> ActiveLightItem;

    // Скорость изменения тепла в секунду
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
    float WarmthRate = 0.1f;

    // Базовый расход энергии в секунду
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
    float BaseEnergyRate = 0.01f;

    // На сколько увеличивается множитель энергии за каждую недостающую единицу тепла
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
    float EnergyMultiplierPerHeat = 0.05f;

    // Для отладки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugMode = false;

private:
    // Таймер для расхода потребностей (раз в секунду)
    float TimeAccumulator = 0.0f;

    // Ссылка на компонент освещения
    class ULight_System_Component* GetLightSystemComponent() const;

    // Расход топлива
    void ConsumeFuel();

    // Обновление тепла
    void UpdateWarmth();

    // Обновление энергии
    void UpdateEnergy();
};