// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank_PC_Base.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Tank_Pawn_Base.h"
#include "InputCoreTypes.h"
#include "Framework/Commands/InputChord.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void ATank_PC_Base::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
        UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CaptureDuringMouseDown);

    bShowMouseCursor = true;

    if (!IsLocalPlayerController())
        return;

    TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/TankU/Blueprints/TankScore.TankScore_C"));
    if (!WidgetClass)
        return;
        
    UUserWidget *MyWidget = CreateWidget<UUserWidget>(this, WidgetClass);
    if (MyWidget)
    {
        MyWidget->AddToViewport();
    }
}

void ATank_PC_Base::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!IsLocalController())
        return;

    ATank_Pawn_Base *ControlledPawn = Cast<ATank_Pawn_Base>(GetPawn());
    if (ControlledPawn)
        ControlledPawn->Aim(TrackMouse());
}

void ATank_PC_Base::OnLeftMouseClick()
{
    ATank_Pawn_Base *ControlledPawn = Cast<ATank_Pawn_Base>(GetPawn());
    if (ControlledPawn)
        ControlledPawn->MoveTo(TrackMouse());
}

void ATank_PC_Base::OnRightMouseClick()
{
    ATank_Pawn_Base *ControlledPawn = Cast<ATank_Pawn_Base>(GetPawn());
    if (ControlledPawn)
        ControlledPawn->Shoot();
}

FVector ATank_PC_Base::TrackMouse()
{
    // Variables for the mouse coordinates
    float MouseX, MouseY;
    FVector WorldLocation;
    FVector WorldDirection;

    // Get the mouse position on the screen
    if (GetMousePosition(MouseX, MouseY))
    {
        // Convert the mouse position to a world space location and direction
        DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);

        // Use the world direction and location to perform your trace
        FVector StartTrace = WorldLocation;
        FVector EndTrace = StartTrace + (WorldDirection * 15000);

        // Perform the line trace
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;

        FCollisionObjectQueryParams ObjectQueryParams;
        ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

        GetWorld()->LineTraceSingleByObjectType(
            HitResult,
            StartTrace,
            EndTrace,
            ObjectQueryParams,
            CollisionParams
        );

        // Check if the line trace hits something
        if (HitResult.bBlockingHit)
        {
            // The trace hit something, return the hit location
            return HitResult.Location;
        }
    }

    // Return an empty vector if nothing was hit or if we failed to get the mouse position
    return FVector::ZeroVector;
}

void ATank_PC_Base::Respawn_Implementation()
{
    if (!GetWorld()) return; // Check for valid world context

    FName DesiredTag = TeamOrange ? FName(TEXT("Orange")) : FName(TEXT("Pink"));
    TArray<AActor *> PlayerStartsWithTag;
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), DesiredTag, PlayerStartsWithTag);

    // Check if there are any player starts with the desired tag
    if (PlayerStartsWithTag.Num() == 0) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnParams.bNoFail = true;

    ATank_Pawn_Base *SpawnedPawn = GetWorld()->SpawnActor<ATank_Pawn_Base>(GetWorld()->GetAuthGameMode()->DefaultPawnClass, PlayerStartsWithTag[0]->GetTransform(), SpawnParams);

    // Check if the pawn was successfully spawned
    if (SpawnedPawn)
        Possess(SpawnedPawn);
}

void ATank_PC_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATank_PC_Base, TeamOrange);
}

void ATank_PC_Base::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ATank_PC_Base::OnLeftMouseClick);
    InputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &ATank_PC_Base::OnRightMouseClick);
}
