// Fill out your copyright notice in the Description page of Project Settings.


#include "TankShell.h"

#include "Tank_Pawn_Base.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Net/UnrealNetwork.h"

#include "DrawDebugHelpers.h"


ATankShell::ATankShell()
{
    PrimaryActorTick.bCanEverTick = false;
    SetReplicates(true);

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;

    CylinderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderComponent"));
    CylinderComponent->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = RootComponent;

    PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    PointLightComponent->SetupAttachment(RootComponent);

    SphereComponent->OnComponentHit.AddDynamic(this, &ATankShell::OnComponentHit);
    PointLightComponent->SetLightColor(TeamOrange ? FLinearColor(255, 203, 89) : FLinearColor(255, 137, 255));
}

void ATankShell::BeginPlay()
{
    Super::BeginPlay();

    PointLightComponent->SetLightColor(TeamOrange ? FLinearColor(1.0, 0.8, 0.3) : FLinearColor(1.0, 0.5, 1.0));

    if (TeamOrange)
    {
        UMaterial *OrangeMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/TankU/Materials/Team_Orange.Team_Orange"));
        if (OrangeMaterial)
        {
            SphereComponent->SetMaterial(0, OrangeMaterial);
            CylinderComponent->SetMaterial(0, OrangeMaterial);
        }
    }
}

void ATankShell::OnComponentHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    if (HasAuthority())
    {
        ATank_Pawn_Base *HitTank = Cast<ATank_Pawn_Base>(OtherActor);
        if (HitTank != nullptr)
            HitTank->BlowUp();
    }

    Destroy();
}

void ATankShell::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATankShell, TeamOrange);
}