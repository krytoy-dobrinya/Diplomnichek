// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_MyCharacter.generated.h" // ВАЖНО: этот include должен быть последним

UCLASS()
class DIPLOMNICHEK_API ACPP_MyCharacter : public ACharacter
{
    GENERATED_BODY()

private:
    // Переменные перемещения персонажа
    bool bForward = 0.0f;
    bool bBack    = 0.0f;
    bool bRight   = 0.0f;
    bool bLeft    = 0.0f;

    // Функция передвижения персонажа
    void ApplyMovement();

public:
    ACPP_MyCharacter();

    virtual void Tick(float DeltaTime) override;

    // Функции вызова каждой клавиши
    UFUNCTION(BlueprintCallable, Category = "Movement") void CPP_Forward(bool bPressed);
    UFUNCTION(BlueprintCallable, Category = "Movement") void CPP_Back(bool bPressed);
    UFUNCTION(BlueprintCallable, Category = "Movement") void CPP_Right(bool bPressed);
    UFUNCTION(BlueprintCallable, Category = "Movement") void CPP_Left(bool bPressed);
};