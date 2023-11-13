// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank_Pawn_Base.generated.h"

UCLASS()
class TANKU_API ATank_Pawn_Base : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default", Replicated)
	bool HasValidDestination;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default", Replicated)
	FVector Destination;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default", Replicated, ReplicatedUsing = OnRep_TeamOrange, meta = (ExposeOnSpawn = "true"))
	bool TeamOrange;

	// Sets default values for this pawn's properties
	ATank_Pawn_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Default")
	void OnRep_TeamOrange();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Destroyed() override;

	UFUNCTION(Server, Reliable)
	void Aim(FVector Direction);
	UFUNCTION(NetMulticast, Reliable)
	void Aim_Local(FVector Direction);
	UFUNCTION(BlueprintImplementableEvent)
	void Aim_BP(FVector Direction);

	UFUNCTION(Server, Reliable)
	void BlowUp();
	UFUNCTION(NetMulticast, Reliable)
	void BlowUp_Local();

	UFUNCTION(Server, Reliable)
	void MoveTo(FVector Direction);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveBy(const TArray<FVector> &PathPoints);
	UFUNCTION(NetMulticast, Reliable)
	void DrawPath(const TArray<FVector> &PathPoints);

	UFUNCTION(Server, Reliable)
	void Shoot();
	UFUNCTION(BlueprintImplementableEvent)
	void Shoot_BP();


protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
