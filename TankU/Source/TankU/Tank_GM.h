// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tank_GM.generated.h"

/**
 * 
 */
UCLASS()
class TANKU_API ATank_GM : public AGameModeBase
{
	GENERATED_BODY()

	ATank_GM();

	//void R(ATank_Base_Pawn *Destroyed);

	virtual AActor* ChoosePlayerStart_Implementation(AController *Player) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController *NewPlayer, const FTransform &SpawnTransform);

	//virtual void RestartPlayerAtPlayerStart(AController *NewPlayer, AActor *StartSpot) override;
};
