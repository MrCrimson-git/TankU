// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PointLightComponent.h"
#include "TankShell.generated.h"

UCLASS()
class TANKU_API ATankShell : public AActor
{
    GENERATED_BODY()

public:
    ATankShell();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION()
    void OnComponentHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent *SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent *CylinderComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    UProjectileMovementComponent *ProjectileMovement;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UPointLightComponent *PointLightComponent;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Configuration", meta = (ExposeOnSpawn = true))
    bool TeamOrange;
};