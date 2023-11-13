// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Tank_GS.generated.h"

/**
 * 
 */
UCLASS()
class TANKU_API ATank_GS : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default", Replicated)
	int OrangePoints = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default", Replicated)
	int PinkPoints = 0;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
