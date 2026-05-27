// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GameUserSettings.h"

#include "CPP_GameUserSettings.h"

UCPP_GameUserSettings* UCPP_GameUserSettings::GetCPP_GameUserSettings()
{
    return Cast<UCPP_GameUserSettings>(UGameUserSettings::GetGameUserSettings());
}