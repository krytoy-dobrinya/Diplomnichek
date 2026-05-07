#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_Light_System_Component.generated.h"

class ACPP_Light_Source;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIPLOMNICHEK_API ULight_System_Component : public UActorComponent
{
    GENERATED_BODY()

public:
    ULight_System_Component();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light Health")
    int32 LightCounter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugMode = true;

    void RecalculateLightCounter();
    void IncrementCounter();
    void DecrementCounter();
    bool HasLineOfSightToPlayer(ACPP_Light_Source* LightSource) const;
};