#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_Light_System_Component.generated.h"

class ACPP_Light_Source;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DIPLOMNICHEK_API ULight_System_Component : public UActorComponent
{
    GENERATED_BODY()

public:
    ULight_System_Component();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    // Счётчик активных источников света
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light System")
    int32 LightCounter = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light System")
    int32 Safe_Zone = 0;

    // Сколько секунд до смерти (константа)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light System")
    float DeathTimer = 3.0f;

    // Текущий таймер
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light System")
    float CurrentDeathTimer = 0.0f;

    // Активен ли таймер
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light System")
    bool bIsDying = false;

    // Режим отладки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugMode = true;

    // Событие: игрок умер
    void OnPlayerDied();

    // Функция для пересчёта счётчика
    UFUNCTION(BlueprintCallable, Category = "Light System")
    void RecalculateLightCounter();

    // Добавляет очко к счётчику (вызывается из CPP_Light_Source)
    void IncrementCounter();

    // Убавляет очко из счётчика (вызывается из CPP_Light_Source)
    void DecrementCounter();

    // Внутренняя проверка, есть ли прямая видимость между источником и игроком
    bool HasLineOfSightToPlayer(ACPP_Light_Source* LightSource) const;

    // Проверка всех источников света при старте игры
    UFUNCTION(BlueprintCallable, Category = "Light System")
    void CheckAllLightSourcesAtStart();

private:
    // Список всех источников света на уровне
    UPROPERTY()
    TArray<ACPP_Light_Source*> AllLightSources;

    // Флаг что игрок уже умер
    bool bHasDied = false;

    void StartDeathTimer();
    void StopDeathTimer();
};