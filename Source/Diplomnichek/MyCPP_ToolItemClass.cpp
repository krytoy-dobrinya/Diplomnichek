// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCPP_ToolItemClass.h"
#include "AFarmingManager.h"
#include "Kismet/GameplayStatics.h"

bool AMyCPP_ToolItemClass::UseItem_Implementation(AActor* User, FVector TargetLocation)
{
    // Находим FarmingManager на уровне
    AFarmingManager* FM = AFarmingManager::GetClosestFarmingManager(GetWorld(), TargetLocation);

    if (!FM)
        return false;

    switch (CPP_Tool_type)
    {
        
    case ECPP_ToolType::Hoe:
        FM->TryTillAtLocation(TargetLocation);
        // TODO: ConsumeEnergy(CPP_Energy_cost)
        return true;
    case ECPP_ToolType::WateringCan:
        FM->TryWaterAtLocation(TargetLocation);
        // TODO: ConsumeEnergy(CPP_Energy_cost)
        return true;

    case ECPP_ToolType::Axe:
    case ECPP_ToolType::Pickaxe:
        return false;

    default:
        return false;
    }
}