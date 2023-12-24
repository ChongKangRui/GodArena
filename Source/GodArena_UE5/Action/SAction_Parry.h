// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "../StructAndEnum.h"
#include "SAction_Parry.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_Parry : public USAction
{
	GENERATED_BODY()
public:
	void Init(TObjectPtr <class AGodsArenaCharacter> owner, FCharacterCombat combatStruct) override;
	void OnActionBegin_Implementation() override;
	void OnActionEnd_Implementation() override;

	//Use to end parry state early
	UPROPERTY(EditDefaultsOnly)
		float ParryDurationOffset = 1.0f;
	
protected:
	FTimerHandle ParryTimerHandle;
	bool IsPlayer;
};
