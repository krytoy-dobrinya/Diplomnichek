// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CPP_GameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class DIPLOMNICHEK_API UCPP_GameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
    UPROPERTY(Config, BlueprintReadWrite)
    float MasterVolume = 1.0f;

    UPROPERTY(Config, BlueprintReadWrite)
    float MusicVolume = 1.0f;

    UPROPERTY(Config, BlueprintReadWrite)
    float SFXVolume = 1.0f;

    UFUNCTION(BlueprintCallable, Category = "Settings")
    static UCPP_GameUserSettings* GetCPP_GameUserSettings();
};
