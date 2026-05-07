// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CPP_Light_Source.generated.h"

UCLASS()
class DIPLOMNICHEK_API ACPP_Light_Source : public AActor
{
    GENERATED_BODY()

public:
    ACPP_Light_Source();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Радиус коллизии
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
    float LightRadius = 900.0f;

    // Высота коллизии относительно актора
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
    float LightCollisionHeight = 150.0f;

private:
    UPROPERTY()
    USphereComponent* LightCollision;

    void CreateLightCollision();

    UFUNCTION()
    void OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnPlayerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY()
    TWeakObjectPtr<AActor> CurrentPlayer;

    bool bIsPlayerInRange = false;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bShowDebugRay = true;

    FVector PlayerLocation;
};
