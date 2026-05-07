// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_Light_System_Component.generated.h"


class UPrimitiveComponent;
class AActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DIPLOMNICHEK_API ULightHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULightHealthComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;


    // Количество источников света, которые сейчас освещают персонажа
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light Health")
    int32 LightCounter = 0;

    // Флаг неуязвимости (когда источник света в руке)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light Health")
    bool bInvincibleFlag = false;

    // Время до смерти в темноте (в секундах)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Health")
    float DeathTime = 10.0f;

    // Текущий таймер до смерти
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light Health")
    float DeathTimer = 0.0f;

    // Активен ли таймер смерти
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light Health")
    bool bDeathTimerActive = false;


    // Добавить источник света
    UFUNCTION(BlueprintCallable, Category = "Light Health")
    void AddLightSource(AActor* LightSource);

    // Удалить источник света
    UFUNCTION(BlueprintCallable, Category = "Light Health")
    void RemoveLightSource(AActor* LightSource);

    // Установить флаг неуязвимости
    UFUNCTION(BlueprintCallable, Category = "Light Health")
    void SetInvincibleFlag(bool bNewFlag);

    // Проверка всех источников света при старте
    UFUNCTION(BlueprintCallable, Category = "Light Health")
    void CheckAllLightSources();

    // Событие: игрок умер от темноты
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDiedInDarkness);
    UPROPERTY(BlueprintAssignable, Category = "Light Health")
    FOnPlayerDiedInDarkness OnPlayerDiedInDarkness;

    // Событие: таймер смерти обновлён (для UI/эффектов)
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathTimerUpdated, float, CurrentTime);
    UPROPERTY(BlueprintAssignable, Category = "Light Health")
    FOnDeathTimerUpdated OnDeathTimerUpdated;

private:
    // Список активных источников света
    UPROPERTY()
    TArray<TWeakObjectPtr<AActor>> ActiveLightSources;

    // Запустить таймер смерти
    void StartDeathTimer();

    // Остановить таймер смерти
    void StopDeathTimer();
};
