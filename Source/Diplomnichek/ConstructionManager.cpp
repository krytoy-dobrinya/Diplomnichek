#include "ConstructionManager.h"
#include "BuildingData.h"
#include "ABuilding.h"
#include "Kismet/GameplayStatics.h"
#include "AFarmingManager.h"

void AConstructionManager::BeginPlay()
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(true);
}

AConstructionManager::AConstructionManager()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AConstructionManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsPlacing)
    {
        UpdateGhostPosition();
    }
}

void AConstructionManager::StartPlacing(UBuildingData* Data)
{
    if (!Data || !Data->BuildingClass)
        return;

    CurrentBuildingData = Data;
    bIsPlacing = true;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GhostActor = GetWorld()->SpawnActor<AActor>(Data->BuildingClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    if (GhostActor)
    {
        UStaticMeshComponent* Mesh = GhostActor->FindComponentByClass<UStaticMeshComponent>();
        if (Mesh)
        {
            Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            Mesh->SetMaterial(0, ValidMaterial);
        }
    }

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        APawn* Pawn = PC->GetPawn();
        if (Pawn)
        {
            Pawn->DisableInput(PC);
        }
        EnableInput(PC);
        PC->SetViewTargetWithBlend(this, 0.0f);
    }
}

void AConstructionManager::UpdateGhostPosition()
{
    if (!GhostActor) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    FHitResult Hit;
    PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        FVector Pos = Hit.Location;
        Pos.X = FMath::RoundToFloat(Pos.X / PlacementGridSize) * PlacementGridSize;
        Pos.Y = FMath::RoundToFloat(Pos.Y / PlacementGridSize) * PlacementGridSize;
        Pos.Z = GhostHeight;

        GhostActor->SetActorLocation(Pos);

        UStaticMeshComponent* Mesh = GhostActor->FindComponentByClass<UStaticMeshComponent>();
        if (Mesh)
        {
            if (CanPlaceAtLocation(Pos))
                Mesh->SetMaterial(0, ValidMaterial);
            else
                Mesh->SetMaterial(0, InvalidMaterial);
        }
    }
}

bool AConstructionManager::CanPlaceAtLocation(FVector Location) const
{
    return true;
}

void AConstructionManager::ConfirmPlacement()
{
    if (!CurrentBuildingData || !GhostActor) return;

    FVector Location = GhostActor->GetActorLocation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AActor* NewBuilding = GetWorld()->SpawnActor<AActor>(
        CurrentBuildingData->BuildingClass,
        Location,
        FRotator::ZeroRotator,
        SpawnParams
    );

    GhostActor->Destroy();
    GhostActor = nullptr;
    CurrentBuildingData = nullptr;
    bIsPlacing = false;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        if (PC->GetPawn())
        {
            PC->SetViewTargetWithBlend(PC->GetPawn(), 0.0f);
            PC->GetPawn()->EnableInput(PC);
        }
        DisableInput(PC);
    }
}

void AConstructionManager::CancelPlacement()
{
    if (GhostActor)
    {
        GhostActor->Destroy();
        GhostActor = nullptr;
    }
    CurrentBuildingData = nullptr;
    bIsPlacing = false;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        if (PC->GetPawn())
        {
            PC->SetViewTargetWithBlend(PC->GetPawn(), 0.0f);
            PC->GetPawn()->EnableInput(PC);
        }
        DisableInput(PC);
    }
}