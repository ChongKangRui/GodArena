// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "SAction_ExecuteEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_ExecuteEnemy : public USAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExecuteDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ApplyDamageDelay = 4.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The offset to end action"))
	float EndActionOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The timer that set enemy to have hit reaction state"))
		float EnemyDamageReactionState_Timer = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The timer that set enemy to executed lay out state"))
		float EnemyExecutedState_Timer = 7.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The timer that set enemy to standing up state"))
		float EnemyStandingUpState_Timer = 9.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The timer that set enemy to idle state which allow to do action"))
		float EnemyIdleState_Timer = 12.0f;

public:
	
	virtual void OnActionBegin_Implementation() override;
	virtual void OnActionEnd_Implementation() override;
	
	void ExecuteTarget(AGodsArenaCharacter* Target);
	void EnemyExecutedState(float Timer_Duration, AGodsArenaCharacter* TargetToSetState,EExecutedState execState);
private:
	FTimerHandle ExecuteMovementHandle;
	
};
