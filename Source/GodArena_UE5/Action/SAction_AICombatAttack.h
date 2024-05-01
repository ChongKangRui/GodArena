// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Engine/EngineTypes.h"
#include "SAction_AICombatAttack.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_AICombatAttack : public USAction
{
	GENERATED_BODY()
public:
	virtual void OnActionBegin_Implementation() override;
	virtual void OnActionEnd_Implementation() override;
	
	virtual void PlayAttackMontage(const int& combatInfoInt);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove = true;

protected:
	FTimerHandle MoveHandle;

	
};
