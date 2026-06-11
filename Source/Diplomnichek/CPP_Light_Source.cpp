#include "CPP_Light_Source.h"
#include "CPP_Light_System_Component.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

ACPP_Light_Source::ACPP_Light_Source()
{
    PrimaryActorTick.bCanEverTick = true;
}


// При начале игры
void ACPP_Light_Source::BeginPlay()
{
    Super::BeginPlay();

    if (!LightCollision)
    {
        CreateLightCollision();

        if (LightCollision)
        {
            LightCollision->SetRelativeLocation(FVector(0.0f, 0.0f, LightCollisionHeight));

            LightCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Light_Source::OnPlayerEnter);
            LightCollision->OnComponentEndOverlap.AddDynamic(this, &ACPP_Light_Source::OnPlayerExit);
        }
    }
}

// Создание коллизии, для проверки близости игрока
void ACPP_Light_Source::CreateLightCollision()
{
    LightCollision = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("LightCollision"));
    LightCollision->RegisterComponent();
    LightCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    LightCollision->SetSphereRadius(LightRadius + 50.0f);                 // Пол метра запаса
    LightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);    // Включить коллизию
    LightCollision->SetCollisionResponseToAllChannels(ECR_Ignore);        // Включить все каналы коллизии
    LightCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Что бы реагировало только на Pawn

    // Работает, если включен дебаг
    LightCollision->SetVisibility(bShowDebugRay);
    LightCollision->SetHiddenInGame(!bShowDebugRay);
}


// При начале игры делает проверку находится ли игрок уже в коллизии
void ACPP_Light_Source::SimulatePlayerEnter(AActor* Player)
{
    if (Player && Player->IsA(ACharacter::StaticClass()))
    {
        CurrentPlayer = Player;
        bIsPlayerInRange = true;
        PlayerLocation = Player->GetActorLocation();
    }
}


// Выполняется каждый тик (кадр)
void ACPP_Light_Source::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Если игрок в радиусе источника света
    if (bIsPlayerInRange && CurrentPlayer.IsValid() && LightCollision)
    {
        PlayerLocation = CurrentPlayer->GetActorLocation(); // Где находится игрок

        // Направляет луч на игрока
        FVector Start = LightCollision->GetComponentLocation(); 
        FVector End = PlayerLocation;

        // Настройки луча: игнорируем себя, используем простую коллизию
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        QueryParams.bTraceComplex = false;

        // Пускаем луч от источника к игроку по каналу LightTrace (кастомный канал)
        FHitResult HitResult;
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult, Start, End, ECC_GameTraceChannel1, QueryParams);

        bool bNowVisible = false; // Проверяет видит ли луч игрока
        if (bHit)
        {
            bNowVisible = (HitResult.GetActor() == CurrentPlayer.Get());
        }
        else
        {
            bNowVisible = true;
        }

        // Смена флага: касается
        if (bNowVisible && !bWasPlayerVisible)
        {
            ULight_System_Component* LightComp = CurrentPlayer->FindComponentByClass<ULight_System_Component>();
            if (LightComp)
            {
                LightComp->IncrementCounter(); // Прибавляем кол-во источников освещения
            }
        }
        // Смена флага: не касается
        else if (!bNowVisible && bWasPlayerVisible)
        {
            ULight_System_Component* LightComp = CurrentPlayer->FindComponentByClass<ULight_System_Component>();
            if (LightComp)
            {
                LightComp->DecrementCounter(); // Вычитаем кол-во источников освещения
            }
        }
        bWasPlayerVisible = bNowVisible;

        // Отладочный луч (если включен дебаг)
        if (bShowDebugRay)
        {
            FColor RayColor = bNowVisible ? FColor::Green : FColor::Red;
            FVector RayEnd = bHit ? HitResult.Location : End;

            DrawDebugLine(GetWorld(), Start, RayEnd, RayColor, false, 0.0f, 0, 3.0f);
            DrawDebugPoint(GetWorld(), RayEnd, 15.0f,
                bNowVisible ? FColor::Green : FColor::Orange, false, 0.0f);
        }
    }
    else
    {
        bWasPlayerVisible = false;
    }
}


// Если игрок вошел в коллизию
void ACPP_Light_Source::OnPlayerEnter(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass())) // Получаем позицию игрока для отправки в него луча
    {
        CurrentPlayer = OtherActor;
        bIsPlayerInRange = true;
        PlayerLocation = OtherActor->GetActorLocation(); 
    }
}


// Если игрок вышел из коллизии
void ACPP_Light_Source::OnPlayerExit(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == CurrentPlayer.Get())
    {
        // Если игрок был видим — вычитаем счётчик
        if (bWasPlayerVisible)
        {
            ULight_System_Component* LightComp = OtherActor->FindComponentByClass<ULight_System_Component>();
            if (LightComp)
            {
                LightComp->DecrementCounter(); // Вычитаем кол-во источников освещения
            }
        }

        CurrentPlayer.Reset();
        bIsPlayerInRange = false;
        bWasPlayerVisible = false;
    }
}