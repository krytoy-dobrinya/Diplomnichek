// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_MyCharacter.generated.h" // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ!

UCLASS()
class DIPLOMNICHEK_API ACPP_MyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACPP_MyCharacter();
    
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void CPP_Move_func(FVector2D Direction);
};