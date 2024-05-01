// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction_Ability_WeaponRangeExtend.h"
#include "Engine/EngineTypes.h"
#include "SAction_KnightAbility02.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_KnightAbility02 : public USAction_Ability_WeaponRangeExtend
{
	GENERATED_BODY()
public:
	virtual void OnActionBegin_Implementation() override;

	UFUNCTION()
	void OnBuffEndBind();

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABuff_Base> ShieldBuff;

	UPROPERTY(EditDefaultsOnly)
	float SpawnDelay = 1.0f;

	ABuff_Base* BuffActor;

private:
	void StunEnemy();


};
