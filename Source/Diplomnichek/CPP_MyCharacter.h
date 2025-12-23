// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_MyCharacter.generated.h" // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ!

UCLASS()
class DIPLOMNICHEK_API ACPP_MyCharacter : public ACharacter
{
    GENERATED_BODY()

private:
    bool Invulnerability = false;     // Неуязвимость темноте (Если есть в руках источник света)
    bool bDeathTimerActive = false;   // Флаг таймера смерти
    float DeathDelay = 2.0f;          // Cчетчик до смерти (В секундах)
    FTimerHandle DeathTimerHandle;    // Таймер смерти

public:
    ACPP_MyCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    int lighting_sources_counter = 0; // Количество источников света

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void CPP_MoveFunc(FVector2D Direction);

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void CPP_StartDeathTimer();

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void CPP_StopDeathTimer();

    void CPP_DeathTimerComplete();

    virtual void Tick(float DeltaTime) override;
};