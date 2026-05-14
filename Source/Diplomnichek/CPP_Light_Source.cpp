#include "CPP_Light_Source.h"
#include "CPP_Light_System_Component.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

ACPP_Light_Source::ACPP_Light_Source()
{
    PrimaryActorTick.bCanEverTick = true;
}

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

void ACPP_Light_Source::CreateLightCollision()
{
    LightCollision = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("LightCollision"));
    LightCollision->RegisterComponent();
    LightCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    LightCollision->SetSphereRadius(LightRadius + 50.0f);
    LightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LightCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    LightCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


    LightCollision->SetVisibility(bShowDebugRay);
    LightCollision->SetHiddenInGame(!bShowDebugRay);
}

void ACPP_Light_Source::SimulatePlayerEnter(AActor* Player)
{
    if (Player && Player->IsA(ACharacter::StaticClass()))
    {
        CurrentPlayer = Player;
        bIsPlayerInRange = true;
        PlayerLocation = Player->GetActorLocation();
    }
}

void ACPP_Light_Source::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsPlayerInRange && CurrentPlayer.IsValid() && LightCollision)
    {
        PlayerLocation = CurrentPlayer->GetActorLocation();

        FVector Start = LightCollision->GetComponentLocation();
        FVector End = PlayerLocation;

        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        QueryParams.bTraceComplex = false;

        FHitResult HitResult;
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult, Start, End, ECC_GameTraceChannel1, QueryParams);

        bool bNowVisible = false;
        if (bHit)
        {
            bNowVisible = (HitResult.GetActor() == CurrentPlayer.Get());
        }
        else
        {
            bNowVisible = true;
        }

        // Смена флага: НЕ касается → КАСАЕТСЯ (+1)
        if (bNowVisible && !bWasPlayerVisible)
        {
            ULight_System_Component* LightComp = CurrentPlayer->FindComponentByClass<ULight_System_Component>();
            if (LightComp)
            {
                LightComp->IncrementCounter();
            }
        }
        // Смена флага: КАСАЕТСЯ → НЕ касается (-1)
        else if (!bNowVisible && bWasPlayerVisible)
        {
            ULight_System_Component* LightComp = CurrentPlayer->FindComponentByClass<ULight_System_Component>();
            if (LightComp)
            {
                LightComp->DecrementCounter();
            }
        }
        bWasPlayerVisible = bNowVisible;

        // Отладочный луч
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
        // Если игрок был видим — вычитаем счётчик
        if (bWasPlayerVisible)
        {
            ULight_System_Component* LightComp = OtherActor->FindComponentByClass<ULight_System_Component>();
            if (LightComp)
            {
                LightComp->DecrementCounter();
            }
        }

        CurrentPlayer.Reset();
        bIsPlayerInRange = false;
        bWasPlayerVisible = false;
    }
}