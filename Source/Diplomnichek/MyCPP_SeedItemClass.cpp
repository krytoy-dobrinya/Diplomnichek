// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCPP_SeedItemClass.h"
#include "AFarmingManager.h"
#include "Kismet/GameplayStatics.h"

bool AMyCPP_SeedItemClass::UseItem_Implementation(AActor* User, FVector TargetLocation)
{
    
    AFarmingManager* FM = AFarmingManager::GetClosestFarmingManager(GetWorld(), TargetLocation);

    if (!FM)
        return false;

    FM->TryPlantAtLocation(TargetLocation, CPP_Plant_ID);
    return true;
}