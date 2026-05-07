// CPP_Light_System_Component.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_Light_System_Component.generated.h"

// Forward declaration для класса источника света
class ACPP_Light_Source;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIPLOMNICHEK_API ULight_System_Component : public UActorComponent
{
    GENERATED_BODY()

public:
    ULight_System_Component();

protected:
    virtual void BeginPlay() override;

public:
    // Счётчик активных источников света
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light Health")
    int32 LightCounter = 0;

    // Функция для пересчёта счётчика
    // Вызывается извне, когда игрок входит/выходит из зоны света
    UFUNCTION(BlueprintCallable, Category = "Light Health")
    void RecalculateLightCounter();

    // Добавляет очко к счётчику (вызывается из CPP_Light_Source при входе игрока)
    void IncrementCounter();

    // Убавляет очко из счётчика (вызывается из CPP_Light_Source при выходе игрока)
    void DecrementCounter();

    // Внутренняя проверка, есть ли прямая видимость между источником и игроком
    bool HasLineOfSightToPlayer(ACPP_Light_Source* LightSource) const;
};