// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank_Pawn_Base.h"

#include "Tank_PC_Base.h"
#include "Tank_GS.h"

#include "Net/UnrealNetwork.h"

#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/GameModeBase.h"

#include "DrawDebugHelpers.h"

// Sets default values
ATank_Pawn_Base::ATank_Pawn_Base()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATank_Pawn_Base::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATank_Pawn_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasValidDestination || GetLifeSpan())
		return;

	UNavigationPath *NavPath = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld())->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), Destination, this, nullptr);
	if (!NavPath || !NavPath->IsValid() || NavPath->PathPoints.Num() <= 1)
		return;

	TArray<FVector> &points = NavPath->PathPoints;

	if (HasAuthority())
	{
		MoveBy(NavPath->PathPoints);
		DrawPath(NavPath->PathPoints);
	}
}

void ATank_Pawn_Base::DrawPath_Implementation(const TArray<FVector> &PathPoints)
{
	for (int i = 0; i < PathPoints.Num() - 1; ++i)
	{
		FVector start = PathPoints[i] + FVector(0, 0, 20);
		FVector end = PathPoints[i + 1] + FVector(0, 0, 20);
		FColor color = TeamOrange ? FColor(255, 175, 0, 255) : FColor(255, 147, 210, 255);
		DrawDebugLine(GetWorld(), start, end, color, false, 0.f, 0, 2);
	}
}

void ATank_Pawn_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank_Pawn_Base::Destroyed()
{
	if (!HasAuthority())
		return;

	ATank_PC_Base *controller = Cast<ATank_PC_Base>(GetController());
	if (controller)
		controller->Respawn();
}

void ATank_Pawn_Base::Aim_Implementation(FVector Direction)
{
	Aim_Local(Direction);
}

void ATank_Pawn_Base::Aim_Local_Implementation(FVector Direction)
{
	Aim_BP(Direction);
}

void ATank_Pawn_Base::BlowUp_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("BlowUp_Implementation")));
	if (GetLifeSpan() > 0.f)
		return;

	HasValidDestination = false;

	ATank_GS *gameState = GetWorld()->GetGameState<ATank_GS>();
	if (TeamOrange)
		++gameState->PinkPoints;
	else
		++gameState->OrangePoints;

	SetLifeSpan(3.0f);
	BlowUp_Local();
}

void ATank_Pawn_Base::BlowUp_Local_Implementation()
{
	TArray<UStaticMeshComponent *> StaticMeshComponents;
	GetComponents<UStaticMeshComponent>(StaticMeshComponents);
	for (UStaticMeshComponent *Component : StaticMeshComponents)
		if (Component)
			Component->SetSimulatePhysics(true);
}

void ATank_Pawn_Base::MoveTo_Implementation(FVector Direction)
{
	HasValidDestination = true;
	Destination = Direction;
}

void ATank_Pawn_Base::Shoot_Implementation()
{
	Shoot_BP();
}

void ATank_Pawn_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATank_Pawn_Base, HasValidDestination);
	DOREPLIFETIME(ATank_Pawn_Base, Destination);
	DOREPLIFETIME(ATank_Pawn_Base, TeamOrange);
}