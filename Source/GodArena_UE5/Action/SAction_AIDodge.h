// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Action.h"
#include "Engine/EngineTypes.h"
#include "SAction_AIDodge.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_AIDodge : public USAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Whether wan to increase the dodge distance or not"))
	float dodgeAnimRate = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Rotation Speed"))
	float DodgeInterpToDirectionSpeed = 720.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Delay to play dodge montage"))
	float DelayBeforeDodge = 0.1f;

	virtual void Init(TObjectPtr <class AGodsArenaCharacter> owner, FCharacterCombat combatStruct) override;
	
	virtual void OnActionBegin_Implementation() override;
	virtual void OnActionEnd_Implementation() override;

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> result);
private:
	
	void RotationBeforeDodge(const FRotator& rotateDir);
	float PlayDodgeMontage();

	class AEnemy_Base* EnemyBase;
	FTimerHandle DodgeRotationHandle;
	FTimerHandle DodgeFinishedHandle;

	float alpha;
	float EndDodgeDuration;
	
};
