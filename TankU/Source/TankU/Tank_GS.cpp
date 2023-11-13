// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank_GS.h"



#include "Net/UnrealNetwork.h"

void ATank_GS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATank_GS, OrangePoints);
	DOREPLIFETIME(ATank_GS, PinkPoints);
}