// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank_GM.h"

#include "Tank_Pawn_Base.h"
#include "Tank_PC_Base.h"
#include "Tank_GS.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

ATank_GM::ATank_GM()
{
    static ConstructorHelpers::FClassFinder<APawn> TankPawnClassFinder(TEXT("Blueprint'/Game/TankU/Blueprints/Tank_Pawn.Tank_Pawn_C'"));
    if (TankPawnClassFinder.Succeeded())
    {
        DefaultPawnClass = TankPawnClassFinder.Class;
    }
    else
    {
        // Fallback to your C++ base class if the Blueprint wasn't found
        DefaultPawnClass = ATank_Pawn_Base::StaticClass();
    }

    static ConstructorHelpers::FClassFinder<APlayerController> TankPCClassFinder(TEXT("Blueprint'/Game/TankU/Blueprints/Tank_PC.Tank_PC_C'"));
    if (TankPCClassFinder.Succeeded())
    {
        PlayerControllerClass = TankPCClassFinder.Class;
    }
    else
    {
        // Fallback to your C++ base class if the Blueprint wasn't found
        PlayerControllerClass = ATank_PC_Base::StaticClass();
    }

    GameStateClass = ATank_GS::StaticClass();
}

AActor *ATank_GM::ChoosePlayerStart_Implementation(AController *Player)
{
    ATank_PC_Base *TankPC = Cast<ATank_PC_Base>(Player);
    TankPC->TeamOrange = (GetNumPlayers() % 2) == 0;
    FName DesiredTag = TankPC->TeamOrange ? FName(TEXT("Orange")) : FName(TEXT("Pink"));
    
    TArray<AActor *> PlayerStartsWithTag;
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), DesiredTag, PlayerStartsWithTag);
    return PlayerStartsWithTag[0];
}

APawn *ATank_GM::SpawnDefaultPawnAtTransform_Implementation(AController *NewPlayer, const FTransform &SpawnTransform)
{
    ATank_PC_Base *TankPC = Cast<ATank_PC_Base>(NewPlayer);
    if (!TankPC) return nullptr;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnParams.bNoFail = true;
    ATank_Pawn_Base *SpawnedPawn = GetWorld()->SpawnActor<ATank_Pawn_Base>(DefaultPawnClass, SpawnTransform, SpawnParams);

    return SpawnedPawn;
}