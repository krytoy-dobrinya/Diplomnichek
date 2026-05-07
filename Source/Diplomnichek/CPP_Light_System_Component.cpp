// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CPP_Light_System_Component.h"

#include "CPP_Light_Source.h"

ULightHealthComponent::ULightHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void ULightHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    // При старте проверяем все источники света
    CheckAllLightSources();
}

void ULightHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Обработка таймера смерти
    if (bDeathTimerActive && !bInvincibleFlag)
    {
        DeathTimer -= DeltaTime;
        OnDeathTimerUpdated.Broadcast(DeathTimer);

        if (DeathTimer <= 0.0f)
        {
            DeathTimer = 0.0f;
            bDeathTimerActive = false;
            OnPlayerDiedInDarkness.Broadcast();
        }
    }
}

void ULightHealthComponent::AddLightSource(AActor* LightSource)
{
    if (!LightSource) return;

    // Проверяем, нет ли уже этого источника в списке
    for (const TWeakObjectPtr<AActor>& ExistingSource : ActiveLightSources)
    {
        if (ExistingSource.Get() == LightSource)
            return; // Уже есть
    }

    ActiveLightSources.Add(LightSource);
    LightCounter++;

    UE_LOG(LogTemp, Warning, TEXT("Light source added. Counter: %d"), LightCounter);

    // Если появился свет — останавливаем таймер
    if (LightCounter > 0)
    {
        StopDeathTimer();
    }
}

void ULightHealthComponent::RemoveLightSource(AActor* LightSource)
{
    if (!LightSource) return;

    ActiveLightSources.RemoveAll([LightSource](const TWeakObjectPtr<AActor>& Source) {
        return Source.Get() == LightSource;
    });

    LightCounter = FMath::Max(0, LightCounter - 1);

    UE_LOG(LogTemp, Warning, TEXT("Light source removed. Counter: %d"), LightCounter);

    // Если источников не осталось и нет неуязвимости — запускаем таймер
    if (LightCounter <= 0 && !bInvincibleFlag)
    {
        StartDeathTimer();
    }
}

void ULightHealthComponent::SetInvincibleFlag(bool bNewFlag)
{
    bInvincibleFlag = bNewFlag;

    if (bInvincibleFlag)
    {
        // Включили фонарь — останавливаем таймер
        StopDeathTimer();
    }
    else
    {
        // Выключили фонарь — если нет других источников, запускаем таймер
        if (LightCounter <= 0)
        {
            StartDeathTimer();
        }
    }
}

void ULightHealthComponent::CheckAllLightSources()
{
    // Находим все источники света в мире и проверяем, не пересекается ли персонаж с ними
    TArray<AActor*> FoundLights;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_BaseLightSource::StaticClass(), FoundLights);

    for (AActor* Light : FoundLights)
    {
        // Получаем коллизию источника
        UPrimitiveComponent* LightCollision = Cast<UPrimitiveComponent>(
            Light->GetComponentByClass(UPrimitiveComponent::StaticClass()));

        if (!LightCollision) continue;

        // Получаем капсулу персонажа
        AActor* Owner = GetOwner();
        if (!Owner) continue;

        UPrimitiveComponent* PlayerCapsule = Cast<UPrimitiveComponent>(
            Owner->GetComponentByClass(UPrimitiveComponent::StaticClass()));

        if (!PlayerCapsule) continue;

        // Проверяем пересечение
        if (LightCollision->IsOverlappingComponent(PlayerCapsule))
        {
            AddLightSource(Light);
        }
    }
}

void ULightHealthComponent::StartDeathTimer()
{
    if (bInvincibleFlag) return;

    DeathTimer = DeathTime;
    bDeathTimerActive = true;

    UE_LOG(LogTemp, Warning, TEXT("Death timer started: %.1f seconds"), DeathTime);
}

void ULightHealthComponent::StopDeathTimer()
{
    bDeathTimerActive = false;
    DeathTimer = DeathTime; // Сбрасываем таймер

    UE_LOG(LogTemp, Warning, TEXT("Death timer stopped"));
}
