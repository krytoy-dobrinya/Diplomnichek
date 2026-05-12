#include "ConstructionManager.h"
#include "BuildingData.h"
#include "ABuilding.h"
#include "Kismet/GameplayStatics.h"
#include "AFarmingManager.h"
#include "Engine/OverlapResult.h"
#include "Landscape.h"
#include "Engine/TriggerBase.h"
#include "AGardenCell.h"


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
    if (!GhostActor) return false;

    UStaticMeshComponent* Mesh = GhostActor->FindComponentByClass<UStaticMeshComponent>();
    if (!Mesh) return true;

    FVector MeshOffset = Mesh->GetRelativeLocation();
    FVector CheckLocation = Location + MeshOffset;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GhostActor);
    Params.AddIgnoredActor(this);

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && PC->GetPawn())
        Params.AddIgnoredActor(PC->GetPawn());

    TArray<AActor*> IgnoredActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFarmingManager::StaticClass(), IgnoredActors);
    for (AActor* A : IgnoredActors) Params.AddIgnoredActor(A);
    IgnoredActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGardenCell::StaticClass(), IgnoredActors);
    for (AActor* A : IgnoredActors) Params.AddIgnoredActor(A);

    TArray<FOverlapResult> Overlaps;
    GetWorld()->OverlapMultiByChannel(
        Overlaps,
        CheckLocation,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeBox(Mesh->Bounds.BoxExtent),
        Params
    );

    for (const FOverlapResult& Overlap : Overlaps)
    {
        if (AActor* Actor = Overlap.GetActor())
        {
            FString ClassName = Actor->GetClass()->GetName();
            if (ClassName.Contains(TEXT("Landscape")) || ClassName.Contains(TEXT("Trigger")) || ClassName.Contains(TEXT("SafeZone")))
                continue;

            return false;
        }
    }

    return true;
}

void AConstructionManager::ConfirmPlacement()
{
    if (!CurrentBuildingData || !GhostActor) return;

    FVector Location = GhostActor->GetActorLocation();

    if (CanPlaceAtLocation(Location)) {
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

bool AConstructionManager::HasRequiredItems(UBuildingData* Data) const
{
    /*if (!Data || Data->RequiredItems.Num() == 0)
        return true;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return false;

    ACharacter* Char = PC->GetCharacter();
    if (!Char) return false;

    BPC_Inventory_Component* Inventory = Char->FindComponentByClass<BPC_Inventory_Component>();
    if (!Inventory) return false;

    for (const FRequiredItem& Req : Data->RequiredItems)
    {
        if (!Inventory->HasEnough(Req.ItemID, Req.Quantity))
            return false;
    }*/
    return true;
}