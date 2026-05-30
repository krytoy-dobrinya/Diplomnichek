#include "CPPC_Needs_component.h"
#include "MyCPP_LightingSourceItemClass.h"
#include "CPP_Light_System_Component.h"
#include "Engine/Engine.h"

UCPPC_Needs_component::UCPPC_Needs_component()
{
    PrimaryComponentTick.bCanEverTick = true;
}

ULight_System_Component* UCPPC_Needs_component::GetLightSystemComponent() const
{
    AActor* Owner = GetOwner();
    if (!Owner) return nullptr;
    return Owner->FindComponentByClass<ULight_System_Component>();
}

void UCPPC_Needs_component::ConsumeFuel()
{
    AMyCPP_LightingSourceItemClass* Item = ActiveLightItem.Get();
    if (!Item) return;

    bool bIsLit = Item->CPP_isLit;
    if (bIsLit)
    {
        Item->CPP_Current_Fuel = FMath::Max(0.0f, Item->CPP_Current_Fuel - 0.1f);
        if (Item->CPP_Current_Fuel <= 0.0f)
        {
            Item->CPP_isLit = false;
        }
    }
}

void UCPPC_Needs_component::UpdateWarmth()
{
    ULight_System_Component* LightComp = GetLightSystemComponent();
    if (!LightComp) return;

    bool bHasHeatSource = (LightComp->LightCounter > 0) || (LightComp->Safe_Zone > 0);

    if (bHasHeatSource)
    {
        Warmth = FMath::Min(100.0f, Warmth + WarmthRate);
    }
    else
    {
        Warmth = FMath::Max(0.0f, Warmth - WarmthRate);
    }
}

void UCPPC_Needs_component::UpdateEnergy()
{
    float MissingHeat = FMath::Max(0.0f, 100.0f - Warmth);
    float EnergyMultiplier = 1.0f + (MissingHeat * EnergyMultiplierPerHeat);

    float EnergyConsumption = BaseEnergyRate * EnergyMultiplier;
    Energy = FMath::Max(0.0f, Energy - EnergyConsumption);
}

void UCPPC_Needs_component::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TimeAccumulator += DeltaTime;
    if (TimeAccumulator >= 1.0f)
    {
        TimeAccumulator = 0.0f;

        ConsumeFuel();
        UpdateWarmth();
        UpdateEnergy();
    }

    if (bDebugMode && GEngine)
    {
        AMyCPP_LightingSourceItemClass* Item = ActiveLightItem.Get();

        FString FuelText = Item
            ? FString::Printf(TEXT("%.1f"), Item->CPP_Current_Fuel)
            : TEXT("None");

        FString LitText = (Item && Item->CPP_isLit) ? TEXT("ON") : TEXT("OFF");

        ULight_System_Component* LightComp = GetLightSystemComponent();
        int32 LightCount = LightComp ? LightComp->LightCounter : 0;
        int32 SafeZones = LightComp ? LightComp->Safe_Zone : 0;

        FString DebugText = FString::Printf(
            TEXT("Energy: %.1f | Warmth: %.1f | Fuel: %s | Lit: %s | Light: %d | Safe: %d"),
            Energy, Warmth, *FuelText, *LitText, LightCount, SafeZones);

        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Cyan, DebugText);
    }
}