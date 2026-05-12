#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BPC_Inventory_Component.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DIPLOMNICHEK_API UBPC_Inventory_Component : public UActorComponent
{
    GENERATED_BODY()

public:
    UBPC_Inventory_Component();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
    bool HasEnough(int32 ItemID, int32 Quantity);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
    void RemoveItem(int32 ItemID, int32 Quantity);
};