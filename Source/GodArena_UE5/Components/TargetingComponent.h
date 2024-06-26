// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

class APlayerCharacter;

USTRUCT(BlueprintType)
struct FDodgeSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RotateToEnemyAfterDodge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReFollowAngleMin = -10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReFollowAngleMax = 10.0f;


};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GODARENA_UE5_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTargetingComponent();

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Can toggle target immedially if we have a valid target"))
	void ToggleTarget();

	UFUNCTION(BlueprintCallable)
	void BeginTarget();

	UFUNCTION(BlueprintCallable)
	void EndTarget();

	UFUNCTION(BlueprintPure)
	const bool CheckIsTargeted() {
		return IsTargeted;
	}

	UFUNCTION(BlueprintCallable)
	void AddTarget(AGodsArenaCharacter* Target);

	UFUNCTION(BlueprintCallable)
	void RemoveTarget(AGodsArenaCharacter* Target);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(bool left);

	void SetTargetingPointWidget();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AGodsArenaCharacter> CurrentTarget;

	UPROPERTY(EditAnywhere)
	FDodgeSetting dodgeSetting;

	UPROPERTY(EditAnywhere)
	TArray<ECharacterState> DisableMeshRotationState;

	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 3.0f;

	UPROPERTY(EditAnywhere)
	float minimumTargetHeight = 0.0f;

	UPROPERTY(EditAnywhere)
	float maximumTargetHeight = -30.0f;

protected:
	UFUNCTION()
	void OnPlayerDeath(ECharacterState state);

	// Called when the game starts
	virtual void BeginPlay() override;

	void TargetingTick();

protected:
	TObjectPtr<APlayerCharacter> player;
	TObjectPtr<USceneComponent> CurrentPointWidget;
	TArray<AGodsArenaCharacter*> TargetedList;

	FTimerHandle TargetingHandle;

	float TargetRotationSpeed;
	bool IsTargeted;

	

};
