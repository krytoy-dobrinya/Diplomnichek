#include "AGardenCell.h"
#include "Engine/DataTable.h"
#include "CPP_BaseItemClass.h"

AGardenCell::AGardenCell()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    PlantMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComponent"));
    PlantMeshComponent->SetupAttachment(RootComponent);
    PlantMeshComponent->SetVisibility(false);
}

void AGardenCell::BeginPlay()
{
    Super::BeginPlay();
    UpdateVisuals();
}

bool AGardenCell::Till()
{
    // Till вызывается только при создании, поэтому просто устанавливаем состояние
    CurrentState = ECellState::Tilled_Dry;
    UpdateVisuals();
    OnStateChanged(CurrentState);

    // TODO: ConsumeEnergy(Amount)
    return true;
}

bool AGardenCell::Water()
{
    if (CurrentState != ECellState::Tilled_Dry &&
        CurrentState != ECellState::Tilled_Watered &&
        CurrentState != ECellState::Planted_Dry &&
        CurrentState != ECellState::Planted_Watered)
        return false;

    // Меняем Dry -> Watered, сохраняя тип (пустая/засаженная)
    if (CurrentState == ECellState::Tilled_Dry)
        CurrentState = ECellState::Tilled_Watered;
    else if (CurrentState == ECellState::Planted_Dry)
        CurrentState = ECellState::Planted_Watered;
    // Если уже Watered — ничего не меняем, просто обновляем флаг

    bWateredToday = true;
    UpdateVisuals();

    // TODO: ConsumeEnergy(Amount)
    return true;
}

bool AGardenCell::PlantSeed(int32 SeedItemID, int32 DaysForSeed, int32 DaysForSprout, UDataTable* CropDataTable)
{
    if (CurrentState != ECellState::Tilled_Dry && CurrentState != ECellState::Tilled_Watered)
        return false;

    PlantedCropID = SeedItemID;
    SeedMesh = nullptr;
    SproutMesh = nullptr;
    FinalMesh = nullptr;
    if (CropDataTable)
    {
        TArray<FCropData*> Rows;
        CropDataTable->GetAllRows<FCropData>("", Rows);
        for (FCropData* Row : Rows)
        {
            if (Row && Row->SeedItemID == SeedItemID)
            {
                SeedMesh = Row->SeedMesh;
                SproutMesh = Row->SproutMesh;
                FinalMesh = Row->FinalMesh;
                break;
            }
        }
    }
    GrowthStage = EGrowthStage::Seed;
    DaysWateredInStage = 0;
    DaysForSeedStage = DaysForSeed;
    DaysForSproutStage = DaysForSprout;

    // Сохраняем влажность при посадке
    CurrentState = (CurrentState == ECellState::Tilled_Watered)
        ? ECellState::Planted_Watered
        : ECellState::Planted_Dry;

    PlantMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    UpdateVisuals();
    OnStateChanged(CurrentState);

    // TODO: ConsumeEnergy(Amount)
    return true;
}

bool AGardenCell::Harvest(ACPP_BaseItemClass*& OutItem, UDataTable* CropDataTable)
{
    OutItem = nullptr;

    // Допускаем оба состояния (сухо и полито)
    if ((CurrentState != ECellState::Planted_Dry && CurrentState != ECellState::Planted_Watered)
        || GrowthStage != EGrowthStage::Final)
        return false;

    if (PlantedCropID == -1)
        return false;

    // Спавним предмет
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Ищем класс предмета в DataTable
    TSubclassOf<ACPP_BaseItemClass> ItemClass = nullptr;
    if (CropDataTable)
    {
        TArray<FCropData*> Rows;
        CropDataTable->GetAllRows<FCropData>("", Rows);
        for (FCropData* Row : Rows)
        {
            if (Row && Row->SeedItemID == PlantedCropID)
            {
                ItemClass = Row->HarvestItemClass;
                break;
            }
        }
    }

    // Спавним
    if (ItemClass)
    {
        OutItem = GetWorld()->SpawnActor<ACPP_BaseItemClass>(ItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
    }
    else
    {
        OutItem = GetWorld()->SpawnActor<ACPP_BaseItemClass>(ACPP_BaseItemClass::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
    }

    if (OutItem)
    {
        OutItem->CPP_Item_ID = PlantedCropID;
        OutItem->CPP_stack_size = 1;
    }

    CurrentState = ECellState::Tilled_Dry;
    PlantedCropID = -1;
    GrowthStage = EGrowthStage::Seed;
    DaysWateredInStage = 0;
    bWateredToday = false;

    PlantMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    UpdateVisuals();
    OnStateChanged(CurrentState);

    return true;
}

void AGardenCell::UpdateGrowthAtEndOfDay(UDataTable* CropDataTable)
{
    // Пустые клетки — сохнут
    if (CurrentState == ECellState::Tilled_Watered)
    {
        CurrentState = ECellState::Tilled_Dry;
        bWateredToday = false;
        UpdateVisuals();
        return;
    }

    if (CurrentState != ECellState::Planted_Dry && CurrentState != ECellState::Planted_Watered)
        return;

    // Засажено, но сухо — не растёт
    if (CurrentState == ECellState::Planted_Dry)
    {
        bWateredToday = false;
        UpdateVisuals();
        return;
    }

    // Planted_Watered — растёт
    DaysWateredInStage++;

    int32 DaysNeeded = (GrowthStage == EGrowthStage::Seed) ? DaysForSeedStage :
        (GrowthStage == EGrowthStage::Sprout) ? DaysForSproutStage : 0;

    if (DaysWateredInStage >= DaysNeeded)
    {
        if (GrowthStage == EGrowthStage::Seed)
        {
            GrowthStage = EGrowthStage::Sprout;
            DaysWateredInStage = 0;
        }
        else if (GrowthStage == EGrowthStage::Sprout)
        {
            GrowthStage = EGrowthStage::Final;
        }
    }

    // После роста клетка остаётся влажной? Пусть сохнет
    CurrentState = ECellState::Planted_Dry;
    bWateredToday = false;
    UpdateVisuals();
}

void AGardenCell::UpdateVisuals_Implementation()
{
    // Переопределяется в Blueprint'е (BP_GardenCell).
    // Здесь меняется материал и меш в зависимости от CurrentState и GrowthStage.
}