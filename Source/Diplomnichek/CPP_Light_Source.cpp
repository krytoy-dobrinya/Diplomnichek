// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Light_Source.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

ACPP_Light_Source::ACPP_Light_Source()
{
    PrimaryActorTick.bCanEverTick = true;
    // Ничего не создаём в конструкторе!
}

void ACPP_Light_Source::BeginPlay()
{
    Super::BeginPlay();

    if (!LightCollision)
    {
        CreateLightCollision();

        if (LightCollision)
        {
            // Устанавливаем высоту сферы
            LightCollision->SetRelativeLocation(FVector(0.0f, 0.0f, LightCollisionHeight));

            LightCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Light_Source::OnPlayerEnter);
            LightCollision->OnComponentEndOverlap.AddDynamic(this, &ACPP_Light_Source::OnPlayerExit);
        }
    }
}

void ACPP_Light_Source::CreateLightCollision()
{
    // Создаём сферу как дочерний компонент
    LightCollision = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("LightCollision"));
    LightCollision->RegisterComponent();
    LightCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    // Настройки коллизии
    LightCollision->SetSphereRadius(LightRadius + 50.0f);
    LightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LightCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    LightCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // Видимость для отладки
    LightCollision->SetVisibility(true);
    LightCollision->SetHiddenInGame(false);
}

void ACPP_Light_Source::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsPlayerInRange && CurrentPlayer.IsValid())
    {
        PlayerLocation = CurrentPlayer->GetActorLocation();

        if (bShowDebugRay && LightCollision)
        {
            FVector Start = LightCollision->GetComponentLocation();
            FVector End = PlayerLocation;

            // Настраиваем параметры Line Trace
            FCollisionQueryParams QueryParams;
            QueryParams.AddIgnoredActor(this);  // Игнорируем сам источник света
            QueryParams.bTraceComplex = false;  // Простая коллизия

            FHitResult HitResult;
            bool bHit = GetWorld()->LineTraceSingleByChannel(
                HitResult,
                Start,
                End,
                ECC_GameTraceChannel1,
                QueryParams
            );

            FColor RayColor;
            FVector RayEnd;

            if (bHit)
            {
                // Луч упёрся в препятствие
                RayColor = FColor::Red;
                RayEnd = HitResult.Location;

                // Рисуем точку на месте удара
                DrawDebugPoint(GetWorld(), HitResult.Location, 15.0f, FColor::Orange, false, 0.0f);
            }
            else
            {
                // Луч дошёл до игрока
                RayColor = FColor::Green;
                RayEnd = End;

                // Рисуем точку на игроке
                DrawDebugPoint(GetWorld(), End, 15.0f, FColor::Green, false, 0.0f);
            }

            // Рисуем луч
            DrawDebugLine(GetWorld(), Start, RayEnd, RayColor, false, 0.0f, 0, 3.0f);
        }
    }
}

void ACPP_Light_Source::OnPlayerEnter(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
    {
        CurrentPlayer = OtherActor;
        bIsPlayerInRange = true;
        PlayerLocation = OtherActor->GetActorLocation();
    }
}

void ACPP_Light_Source::OnPlayerExit(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == CurrentPlayer.Get())
    {
        CurrentPlayer.Reset();
        bIsPlayerInRange = false;
    }
}