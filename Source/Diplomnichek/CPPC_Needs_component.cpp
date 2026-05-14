// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPC_Needs_component.h"


UCPPC_Needs_component::UCPPC_Needs_component()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCPPC_Needs_component::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Получаем предмет в руке
    AMyCPP_LightingSourceItemClass* Item = ActiveLightItem.Get();
    bool bHasLight = (Item != nullptr);
    bool bIsLit = bHasLight && Item->CPP_isLit;

    // Раз в секунду обновляем потребности
    TimeAccumulator += DeltaTime;
    if (TimeAccumulator >= 1.0f)
    {
        TimeAccumulator = 0.0f;

        // Энергия тратится всегда
        Energy = FMath::Max(0.0f, Energy - 0.01f);

        // Топливо тратится только если фонарь в руке и включен
        if (bIsLit)
        {
            Item->CPP_Current_Fuel = FMath::Max(0.0f, Item->CPP_Current_Fuel - 0.1f);

            // Если топливо кончилось — выключаем фонарь
            if (Item->CPP_Current_Fuel <= 0.0f)
            {
                Item->CPP_isLit = false;
            }
        }
    }

    // Отладочный вывод
    if (bDebugMode && GEngine)
    {
        FString FuelText = bHasLight
            ? FString::Printf(TEXT("%.1f"), Item->CPP_Current_Fuel)
            : TEXT("None");

        FString LitText = bIsLit ? TEXT("ON") : TEXT("OFF");

        FString DebugText = FString::Printf(
            TEXT("Energy: %.1f | Warmth: %.1f | Fuel: %s | Lit: %s"),
            Energy, Warmth, *FuelText, *LitText);

        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Cyan, DebugText);
    }
}
