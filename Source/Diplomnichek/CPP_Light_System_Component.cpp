#include "CPP_Light_System_Component.h"
#include "CPP_Light_Source.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

ULight_System_Component::ULight_System_Component()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void ULight_System_Component::BeginPlay()
{
    Super::BeginPlay();
    LightCounter = 0;

    CheckAllLightSourcesAtStart();
}

void ULight_System_Component::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Проверяем, в опасности ли игрок
    bool bInDanger = (LightCounter <= 0 && Safe_Zone <= 0);

    if (bInDanger)
    {
        if (!bIsDying)
        {
            StartDeathTimer();
        }
        else
        {
            CurrentDeathTimer -= DeltaTime;
            if (CurrentDeathTimer <= 0.0f)
            {
                CurrentDeathTimer = 0.0f;
                bIsDying = false;
                OnPlayerDied();
            }
        }
    }
    else
    {
        if (bIsDying)
        {
            StopDeathTimer();
        }
    }

    // Отладка
    if (bDebugMode)
    {
        FString DebugText = FString::Printf(TEXT("Light: %d | SafeZones: %d | %s"),
            LightCounter,
            Safe_Zone,
            bIsDying ? *FString::Printf(TEXT("Dying: %.1f"), CurrentDeathTimer) : TEXT("Safe"));
        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow, DebugText);
    }
}

void ULight_System_Component::RecalculateLightCounter()
{
    UE_LOG(LogTemp, Warning, TEXT("Light Counter Updated: %d"), LightCounter);
}

void ULight_System_Component::IncrementCounter()
{
    LightCounter++;
    RecalculateLightCounter();
}

void ULight_System_Component::DecrementCounter()
{
    LightCounter = FMath::Max(0, LightCounter - 1);
    RecalculateLightCounter();
}

bool ULight_System_Component::HasLineOfSightToPlayer(ACPP_Light_Source* LightSource) const
{
    if (!LightSource || !GetOwner()) return false;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(LightSource);
    QueryParams.bTraceComplex = false;

    FVector Start = LightSource->GetActorLocation() + FVector(0, 0, LightSource->LightCollisionHeight);
    FVector End = GetOwner()->GetActorLocation();

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, QueryParams);

    return !bHit || (bHit && HitResult.GetActor() == GetOwner());
}

void ULight_System_Component::CheckAllLightSourcesAtStart()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_Light_Source::StaticClass(), FoundActors);

    AActor* Owner = GetOwner();
    if (!Owner) return;

    for (AActor* Actor : FoundActors)
    {
        ACPP_Light_Source* LightSource = Cast<ACPP_Light_Source>(Actor);
        if (!LightSource) continue;

        float Distance = FVector::Dist(Owner->GetActorLocation(), LightSource->GetActorLocation());
        float LightRadius = LightSource->LightRadius + 50.0f;

        if (Distance <= LightRadius)
        {
            if (HasLineOfSightToPlayer(LightSource))
            {
                AllLightSources.Add(LightSource);

                // Имитируем вход игрока в коллизию
                LightSource->SimulatePlayerEnter(Owner);

                if (bDebugMode)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Start check: light source at distance %.1f illuminates player"), Distance);
                }
            }
        }
    }

    if (bDebugMode)
    {
        UE_LOG(LogTemp, Warning, TEXT("Start check complete. LightCounter: %d"), LightCounter);
    }
}

void ULight_System_Component::StartDeathTimer()
{
    bIsDying = true;
    CurrentDeathTimer = DeathTimer;
}

void ULight_System_Component::StopDeathTimer()
{
    bIsDying = false;
    CurrentDeathTimer = DeathTimer;
}

void ULight_System_Component::OnPlayerDied()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Ты помер!"));
    }
}