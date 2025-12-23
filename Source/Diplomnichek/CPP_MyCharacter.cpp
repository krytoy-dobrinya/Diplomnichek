// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MyCharacter.h"

ACPP_MyCharacter::ACPP_MyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACPP_MyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Проверяем источники света каждый кадр
    if (lighting_sources_counter <= 0)
    {
        // Если нет источников света и мы еще не умираем
        if (!bDeathTimerActive)
        {
            CPP_StartDeathTimer();
            bDeathTimerActive = true;
        }
    }
    else
    {
        // Если есть источники света и таймер смерти активен
        if (bDeathTimerActive)
        {
            CPP_StopDeathTimer();
            bDeathTimerActive = false;
        }
    }
}

void ACPP_MyCharacter::CPP_MoveFunc(FVector2D Direction)
{
    // Движение по осям X и Y
    AddMovementInput(FVector(Direction.X, Direction.Y, 0.0f));
}

void ACPP_MyCharacter::CPP_StartDeathTimer()
{
    if (!bDeathTimerActive)
    {
        bDeathTimerActive = true;
        GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &ACPP_MyCharacter::CPP_DeathTimerComplete, DeathDelay, false);
        GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::White, TEXT("Таймер запущен"));
    }
    
}

void ACPP_MyCharacter::CPP_StopDeathTimer()
{
    if (bDeathTimerActive)
    {
        bDeathTimerActive = false;
        GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
        GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::White, TEXT("Таймер остановлен"));
    }

}

void ACPP_MyCharacter::CPP_DeathTimerComplete()
{
    bDeathTimerActive = false;
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("ВЫ УМЕРЛИ"), true, FVector2D(10.0f, 10.0f)
    );
}