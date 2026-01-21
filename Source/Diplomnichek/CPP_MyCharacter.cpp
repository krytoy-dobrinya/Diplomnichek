// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_MyCharacter.h"



ACPP_MyCharacter::ACPP_MyCharacter()
{
    PrimaryActorTick.bCanEverTick = true; // Включаем возможность вызывать Tick
}


// Вызов каждый кадр
void ACPP_MyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); // Для корректной работы Tick

    ApplyMovement(); // Каждый кадр смотрим изменение клавиш
}


// Функция передвижения персонажа
void ACPP_MyCharacter::ApplyMovement()
{
    // Вычисляем направление
    float X = (bForward ? -1.0f : 0.0f) + (bBack ? 1.0f : 0.0f);
    float Y = (bRight ? -1.0f : 0.0f) + (bLeft ? 1.0f : 0.0f);
    
    FVector Direction(X, Y, 0.0f);
    
    if (!Direction.IsZero())
    {
        // Нормализация вектора (решает проблему скорости по диагонали)
        Direction.Normalize();
        AddMovementInput(Direction, 1.0f);
    }
}


// Функции вызова каждой клавиши
void ACPP_MyCharacter::CPP_Forward(bool bPressed) {bForward = bPressed;}
void ACPP_MyCharacter::CPP_Back(bool bPressed)    {bBack    = bPressed;}
void ACPP_MyCharacter::CPP_Right(bool bPressed)   {bRight   = bPressed;}
void ACPP_MyCharacter::CPP_Left(bool bPressed)    {bLeft    = bPressed;}
