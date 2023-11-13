// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tank_PC_Base.generated.h"

/**
 * 
 */
UCLASS()
class TANKU_API ATank_PC_Base : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default", Replicated, meta = (ExposeOnSpawn = "true"))
	bool TeamOrange;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void OnLeftMouseClick();
	void OnRightMouseClick();
	FVector TrackMouse();

	UFUNCTION(Server, Reliable)
	void Respawn();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
};