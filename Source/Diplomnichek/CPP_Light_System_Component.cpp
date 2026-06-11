#include "CPP_Light_System_Component.h"
#include "CPP_Light_Source.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "AGameTimeManager.h"

ULight_System_Component::ULight_System_Component()
{
    PrimaryComponentTick.bCanEverTick = true;
}


void ULight_System_Component::BeginPlay()
{
    Super::BeginPlay(); 
    LightCounter = 0;   // Для корректной работы еще раз обнулим

    CheckAllLightSourcesAtStart();
}

void ULight_System_Component::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bHasDied) return;  // Если уже умер ничего не делаем

    bool bInDanger = (LightCounter <= 0 && Safe_Zone <= 0 && Invincible_flag == false); // Игрок в темноте

    // Если игрок в темноте
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
                bHasDied = true;  // Блокируем повторный вызов старта нового дня
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

    // Отладка (еслю включен дебаг)
    if (bDebugMode)
    {
        FString DebugText = FString::Printf(TEXT("Light: %d | SafeZones: %d | %s"),
            LightCounter,
            Safe_Zone,
            bIsDying ? *FString::Printf(TEXT("Dying: %.1f"), CurrentDeathTimer) : TEXT("Safe"));
        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow, DebugText);
    }
}

// Вывод в логи (для отладки)
void ULight_System_Component::RecalculateLightCounter()
{
    UE_LOG(LogTemp, Warning, TEXT("Light Counter Updated: %d"), LightCounter);
}

// Увеличить счетчик источников света
void ULight_System_Component::IncrementCounter()
{
    LightCounter++;
    RecalculateLightCounter();
}

// Уменьшить счетчик источников света
void ULight_System_Component::DecrementCounter()
{
    LightCounter = FMath::Max(0, LightCounter - 1);
    RecalculateLightCounter();
}

// Проверяем, есть ли прямая видимость между источником света и игроком (вызывается 1 раз в начале игры)
bool ULight_System_Component::HasLineOfSightToPlayer(ACPP_Light_Source* LightSource) const
{
    // Если нет источника или владельца то выходим
    if (!LightSource || !GetOwner()) return false;

    // Настройки луча: игнорируем источник, простая коллизия
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(LightSource);
    QueryParams.bTraceComplex = false;

    // Начало луча — центр коллизии источника, конец — позиция игрока
    FVector Start = LightSource->GetActorLocation() + FVector(0, 0, LightSource->LightCollisionHeight);
    FVector End = GetOwner()->GetActorLocation();

    // Пускаем луч по каналу LightTrace
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, QueryParams);

    // Игрок видим, если луч не попал ни в что ИЛИ попал прямо в игрока
    return !bHit || (bHit && HitResult.GetActor() == GetOwner());
}

// Проверяем находится ли игрок в источнике света на старте
void ULight_System_Component::CheckAllLightSourcesAtStart()
{
    // Находим все источники света на уровне
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_Light_Source::StaticClass(), FoundActors);

    AActor* Owner = GetOwner();
    if (!Owner) return;

    // Проходим по каждому источнику
    for (AActor* Actor : FoundActors)
    {
        ACPP_Light_Source* LightSource = Cast<ACPP_Light_Source>(Actor);
        if (!LightSource) continue;

        // Проверяем, находится ли игрок в радиусе коллизии источника
        float Distance = FVector::Dist(Owner->GetActorLocation(), LightSource->GetActorLocation());
        float LightRadius = LightSource->LightRadius + 50.0f;

        // Проверяем прямую видимость луча до игрока
        if (Distance <= LightRadius)
        {
            if (HasLineOfSightToPlayer(LightSource))
            {
                AllLightSources.Add(LightSource);

                // Имитируем вход игрока в коллизию
                LightSource->SimulatePlayerEnter(Owner);
            }
        }
    }
}

// Сбросить состояние смерти
void ULight_System_Component::ResetDeathState()
{
    bHasDied = false;
    bIsDying = false;
    CurrentDeathTimer = DeathTimer;
}

// Начать отсчет до смерти
void ULight_System_Component::StartDeathTimer()
{
    bIsDying = true;
    CurrentDeathTimer = DeathTimer;
}

// Остановить отсчет до смерти
void ULight_System_Component::StopDeathTimer()
{
    bIsDying = false;
    CurrentDeathTimer = DeathTimer;
}

// При смерти игрока
void ULight_System_Component::OnPlayerDied()
{
    // Находим TimeManager на уровне
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameTimeManager::StaticClass(), Found);
    if (Found.Num() > 0)
    {
        if (AGameTimeManager* TimeManager = Cast<AGameTimeManager>(Found[0]))
        {
            TimeManager->EndDayDeath(); // Запускаем сценарий смерти (телепорт)
        }
    }
    
    // Задержка перед сбросом состояния смерти
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &ULight_System_Component::ResetDeathState,
        4.0f,   // 4 секунды задержки (время анимации для респавна)
        false
    );
}