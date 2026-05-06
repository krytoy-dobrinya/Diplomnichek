#include "AFarmingManager.h"
#include "AGardenCell.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"

AFarmingManager::AFarmingManager()
{
    PrimaryActorTick.bCanEverTick = false;
    GridOrigin = GetActorLocation();
}

void AFarmingManager::BeginPlay()
{
    Super::BeginPlay();
    GridOrigin = GetActorLocation();
}

FVector AFarmingManager::SnapToGrid(FVector WorldLocation) const
{
    // Переводим в локальные координаты относительно начала сетки
    FVector LocalPos = WorldLocation - GridOrigin;

    // Выравниваем по размеру клетки
    int32 GridX = FMath::RoundToInt(LocalPos.X / CellSize);
    int32 GridY = FMath::RoundToInt(LocalPos.Y / CellSize);

    // Проверяем, не выходит ли за границы
    if (GridX < 0 || GridX >= MaxGridWidth || GridY < 0 || GridY >= MaxGridHeight)
        return FVector::ZeroVector; // Невалидная позиция

    // Возвращаем мировые координаты центра клетки
    return GridOrigin + FVector(GridX * CellSize, GridY * CellSize, 0);
}

AGardenCell* AFarmingManager::FindCellAtGrid(int32 GridX, int32 GridY) const
{
    for (AGardenCell* Cell : Cells)
    {
        if (Cell && Cell->GridX == GridX && Cell->GridY == GridY)
            return Cell;
    }
    return nullptr;
}

AGardenCell* AFarmingManager::TryTillAtLocation(FVector WorldLocation)
{
    FVector SnappedPos = SnapToGrid(WorldLocation);
    if (SnappedPos.IsZero())
        return nullptr; // За границей сетки
    FVector SpawnPos = SnappedPos;
    SpawnPos.Z += 10;

    // Вычисляем координаты в сетке
    FVector LocalPos = SnappedPos - GridOrigin;
    int32 GridX = FMath::RoundToInt(LocalPos.X / CellSize);
    int32 GridY = FMath::RoundToInt(LocalPos.Y / CellSize);

    // Проверяем, нет ли уже клетки здесь
    if (FindCellAtGrid(GridX, GridY))
        return nullptr; // Уже есть грядка — ничего не делаем

    // Спавним новую клетку
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AGardenCell* NewCell = nullptr;
    if (GardenCellClass)
    {
        NewCell = GetWorld()->SpawnActor<AGardenCell>(GardenCellClass, SpawnPos, FRotator::ZeroRotator, SpawnParams);
    }
    else
    {
        NewCell = GetWorld()->SpawnActor<AGardenCell>(AGardenCell::StaticClass(), SpawnPos, FRotator::ZeroRotator, SpawnParams);
    }
    if (NewCell)
    {
        NewCell->GridX = GridX;
        NewCell->GridY = GridY;
        NewCell->Till();
        Cells.Add(NewCell);
    }

    UE_LOG(LogTemp, Warning, TEXT("TryTillAt: %s"), *SnappedPos.ToString());
    return NewCell;
}

AGardenCell* AFarmingManager::TryWaterAtLocation(FVector WorldLocation)
{
    FVector SnappedPos = SnapToGrid(WorldLocation);
    if (SnappedPos.IsZero())
        return nullptr; // За границей сетки
    FVector LocalPos = SnappedPos - GridOrigin;
    int32 GridX = FMath::RoundToInt(LocalPos.X / CellSize);
    int32 GridY = FMath::RoundToInt(LocalPos.Y / CellSize);

    AGardenCell* Cell = FindCellAtGrid(GridX, GridY);
    if (Cell)
    {
        Cell->Water();
    }
    return Cell;
}

AGardenCell* AFarmingManager::TryPlantAtLocation(FVector WorldLocation, int32 SeedItemID)
{
    FVector SnappedPos = SnapToGrid(WorldLocation);
    FVector LocalPos = SnappedPos - GridOrigin;
    int32 GridX = FMath::RoundToInt(LocalPos.X / CellSize);
    int32 GridY = FMath::RoundToInt(LocalPos.Y / CellSize);

    AGardenCell* Cell = FindCellAtGrid(GridX, GridY);
    if (!Cell || !CropDataTable)
        return nullptr;

    // Ищем данные о культуре по SeedItemID
    TArray<FCropData*> Rows;
    CropDataTable->GetAllRows<FCropData>("", Rows);

    for (FCropData* Row : Rows)
    {
        if (Row && Row->SeedItemID == SeedItemID)
        {
            if (Cell->PlantSeed(SeedItemID, Row->DaysForSeed, Row->DaysForSprout))
                return Cell;
        }
    }

    return nullptr;
}

bool AFarmingManager::TryHarvestAtLocation(FVector WorldLocation, ACPP_BaseItemClass*& OutItem)
{
    OutItem = nullptr;

    FVector SnappedPos = SnapToGrid(WorldLocation);
    if (SnappedPos.IsZero())
        return false;

    FVector LocalPos = SnappedPos - GridOrigin;
    int32 GridX = FMath::RoundToInt(LocalPos.X / CellSize);
    int32 GridY = FMath::RoundToInt(LocalPos.Y / CellSize);

    AGardenCell* Cell = FindCellAtGrid(GridX, GridY);
    if (!Cell)
        return false;

    return Cell->Harvest(OutItem, CropDataTable);
}

void AFarmingManager::UpdateAllCellsAtEndOfDay()
{
    for (AGardenCell* Cell : Cells)
    {
        if (Cell)
        {
            Cell->UpdateGrowthAtEndOfDay(CropDataTable);
        }
    }
}