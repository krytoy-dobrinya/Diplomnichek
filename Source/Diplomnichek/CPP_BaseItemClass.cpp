// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseItemClass.h"

// Sets default values
ACPP_BaseItemClass::ACPP_BaseItemClass()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool ACPP_BaseItemClass::UseItem_Implementation(AActor* User, FVector TargetLocation)
{
	return false;
}