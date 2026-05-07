#include "CPP_Light_System_Component.h"
#include "CPP_Light_Source.h"
#include "Engine/World.h"
#include "Engine/Engine.h"  // <-- Добавлен для GEngine

ULight_System_Component::ULight_System_Component()
{
    PrimaryComponentTick.bCanEverTick = true;  // Включаем тик
}

void ULight_System_Component::BeginPlay()
{
    Super::BeginPlay();
    LightCounter = 0;
}

void ULight_System_Component::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bDebugMode)
    {
        FString DebugText = FString::Printf(TEXT("Light Counter: %d"), LightCounter);
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
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.AddIgnoredActor(LightSource);

    FVector Start = LightSource->GetActorLocation();
    FVector End = GetOwner()->GetActorLocation();

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
    return !bHit;
}