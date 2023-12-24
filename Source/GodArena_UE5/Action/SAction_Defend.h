// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "SAction_Defend.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_Defends : public USAction
{
	GENERATED_BODY()

public:
	void Init(TObjectPtr <class AGodsArenaCharacter> owner, FCharacterCombat combatStruct) override;
	void OnActionBegin_Implementation() override;
	void OnActionEnd_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool Blocking;
	
	UFUNCTION()
	void HitReactionBind(AActor* Instigator,float Amount,FAttackDebuff debuff);

protected:
	FTimerHandle Parry_Handle;
	FTimerHandle HitReaction_Handle;
	float OGSpeed;
};
