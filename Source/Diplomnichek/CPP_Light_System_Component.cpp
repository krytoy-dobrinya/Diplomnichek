// CPP_Light_System_Component.cpp
#include "CPP_Light_System_Component.h"
#include "CPP_Light_Source.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

ULight_System_Component::ULight_System_Component()
{
    PrimaryComponentTick.bCanEverTick = false; // Тик пока не нужен
}

void ULight_System_Component::BeginPlay()
{
    Super::BeginPlay();
    
    // При старте просто обнуляем счётчик
    LightCounter = 0;
    
    // TODO: Здесь позже можно добавить проверку всех источников на старте
}

void ULight_System_Component::RecalculateLightCounter()
{
    // Пока что просто обновляем UI/логику
    UE_LOG(LogTemp, Warning, TEXT("Light Counter Updated: %d"), LightCounter);
    
    // Здесь позже будет логика таймера смерти
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

    // Настройки Line Trace (пока используем Visibility канал)
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.AddIgnoredActor(LightSource);

    FVector Start = LightSource->GetActorLocation();
    FVector End = GetOwner()->GetActorLocation();

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
    
    // Если не попали в препятствие, значит, видимость есть
    return !bHit;
}