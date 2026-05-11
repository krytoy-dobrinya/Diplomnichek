#include "AGameTimeManager.h"
#include "AFarmingManager.h"
#include "Kismet/GameplayStatics.h"

AGameTimeManager::AGameTimeManager()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentHour = DayStartHour;
    CurrentDay = 1;
    CurrentMonth = 1;
    CurrentYear = 1;
    bIsTimeRunning = false;
}

void AGameTimeManager::BeginPlay()
{
    Super::BeginPlay();
    StartTime();
}

void AGameTimeManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsTimeRunning)
    {
        AdvanceTime(DeltaTime);
    }
}

void AGameTimeManager::StartTime()
{
    bIsTimeRunning = true;
}

void AGameTimeManager::StopTime()
{
    bIsTimeRunning = false;
}

void AGameTimeManager::AdvanceTime(float DeltaTime)
{
    CurrentHour += DeltaTime / RealSecondsPerGameHour;

    const int32 Hour24 = FMath::FloorToInt(CurrentHour) % 24;
    const int32 Minute = FMath::FloorToInt((CurrentHour - FMath::FloorToInt(CurrentHour)) * 60.0f);
    OnTimeUpdated.Broadcast(Hour24, Minute);

    if (CurrentHour >= DayEndHour + 24.0f)
    {
        EndDay(true);
    }
}

void AGameTimeManager::EndDayEarly()
{
    EndDay(true);
}

void AGameTimeManager::EndDayDeath()
{
    EndDay(false);
}

void AGameTimeManager::EndDay(bool reason)
{
    StopTime();

    // Обновление грядок
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFarmingManager::StaticClass(), Found);
    for (AActor* Actor : Found)
    {
        if (AFarmingManager* FM = Cast<AFarmingManager>(Actor))
        {
            FM->UpdateAllCellsAtEndOfDay();
        }
    }

    OnDayEnded.Broadcast(); // HUD показывает анимацию

    // Запускаем таймер на 4 секунды, потом начинаем новый день
    FTimerHandle TimerHandle;
    if (reason) // Персонаж поспал
    {
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle, this, &AGameTimeManager::StartNewDay, 4.0f, false);
    }
    else        // Персонаж умер
    {
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle, this, &AGameTimeManager::PlayerDeath, 4.0f, false);
    }
}

void AGameTimeManager::StartNewDay()
{
    // Телепортация игрока
    if (!SpawnPoint.IsNull())
    {
        AActor* Spawn = SpawnPoint.LoadSynchronous();
        if (Spawn)
        {
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC && PC->GetPawn())
            {
                PC->GetPawn()->SetActorLocation(Spawn->GetActorLocation());
                PC->GetPawn()->SetActorRotation(Spawn->GetActorRotation());
            }
        }
    }

    if (++CurrentDay > 28)
    {
        CurrentDay = 1;
        if (++CurrentMonth > 4)
        {
            CurrentMonth = 1;
            ++CurrentYear;
        }
    }
    CurrentHour = DayStartHour;
    StartTime();
}

void AGameTimeManager::PlayerDeath()
{
    // Телепортация игрока
    if (!MedicPoint.IsNull())
    {
        AActor* Spawn = MedicPoint.LoadSynchronous();
        if (Spawn)
        {
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC && PC->GetPawn())
            {
                PC->GetPawn()->SetActorLocation(Spawn->GetActorLocation());
                PC->GetPawn()->SetActorRotation(Spawn->GetActorRotation());
            }
        }
    }

    if (++CurrentDay > 28)
    {
        CurrentDay = 1;
        if (++CurrentMonth > 4)
        {
            CurrentMonth = 1;
            ++CurrentYear;
        }
    }
    CurrentHour = DayStartHour;
    StartTime();
}


FString AGameTimeManager::GetTimeString() const
{
    const int32 Hours = FMath::FloorToInt(CurrentHour) % 24;
    const int32 Minutes = FMath::FloorToInt((CurrentHour - FMath::FloorToInt(CurrentHour)) * 60);
    return FString::Printf(TEXT("%02d:%02d"), Hours, Minutes);
}

FString AGameTimeManager::GetDayOfWeek() const
{
    static const TArray<FString> DayNames = {
        TEXT("ПН"), 
        TEXT("ВТ"), 
        TEXT("СР"), 
        TEXT("ЧТ"), 
        TEXT("ПТ"),
        TEXT("СБ"),
        TEXT("ВС") 
    };
    return DayNames[(CurrentDay - 1) % 7];
}