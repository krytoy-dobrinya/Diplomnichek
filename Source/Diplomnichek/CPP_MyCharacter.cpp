// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MyCharacter.h"

ACPP_MyCharacter::ACPP_MyCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ACPP_MyCharacter::CPP_Move_func(FVector2D Direction)
{
    // Движение по осям X и Y
    AddMovementInput(FVector(Direction.X, Direction.Y, 0.0f));
}