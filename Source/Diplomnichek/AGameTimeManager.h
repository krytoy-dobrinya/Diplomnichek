#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGameTimeManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDayEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeUpdated, int32, Hour, int32, Minute);

UCLASS()
class DIPLOMNICHEK_API AGameTimeManager : public AActor
{
    GENERATED_BODY()

public:
    AGameTimeManager();

    UPROPERTY(BlueprintReadOnly, Category = "Time")
    int32 CurrentDay;

    UPROPERTY(BlueprintReadOnly, Category = "Time")
    int32 CurrentMonth;

    UPROPERTY(BlueprintReadOnly, Category = "Time")
    int32 CurrentYear;

    UPROPERTY(BlueprintReadOnly, Category = "Time")
    float CurrentHour;

    UPROPERTY(EditAnywhere, Category = "Time")
    float RealSecondsPerGameHour = 60.0f;

    UPROPERTY(EditAnywhere, Category = "Time")
    float DayStartHour = 6.0f;

    UPROPERTY(EditAnywhere, Category = "Time")
    float DayEndHour = 2.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Time")
    bool bIsTimeRunning;

    UPROPERTY(BlueprintAssignable, Category = "Time")
    FOnDayEnded OnDayEnded;

    UPROPERTY(BlueprintAssignable, Category = "Time")
    FOnTimeUpdated OnTimeUpdated;

    UPROPERTY(EditAnywhere, Category = "Time")
    TSoftObjectPtr<AActor> SpawnPoint; // ссылка на Target Point

    UPROPERTY(EditAnywhere, Category = "Time")
    TSoftObjectPtr<AActor> MedicPoint; // ссылка на Medic Point

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Time")
    void StartTime();

    UFUNCTION(BlueprintCallable, Category = "Time")
    void StopTime();

    UFUNCTION(BlueprintCallable, Category = "Time")
    void EndDayEarly();

    UFUNCTION(BlueprintCallable, Category = "Time")
    void StartNewDay();

    UFUNCTION(BlueprintCallable, Category = "Time")
    void EndDayDeath();

    UFUNCTION(BlueprintCallable, Category = "Time")
    void PlayerDeath();

    UFUNCTION(BlueprintPure, Category = "Time")
    FString GetTimeString() const;
    
    UFUNCTION(BlueprintPure, Category = "Time")
    FString GetDayOfWeek() const;

    UFUNCTION(BlueprintPure, Category = "Time")
    int32 GetCurrentDay() const { return CurrentDay; }

private:

    void AdvanceTime(float DeltaTime);
    void EndDay(bool reason);
};