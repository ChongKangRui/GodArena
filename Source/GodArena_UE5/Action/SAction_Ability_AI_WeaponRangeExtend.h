// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction_AICombatAttack.h"
#include "SAction_Ability_AI_WeaponRangeExtend.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_Ability_AI_WeaponRangeExtend : public USAction_AICombatAttack
{
	GENERATED_BODY()
public:
	//~USAction_Ability_WeaponRangeExtend();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "In Relative position"))
	FVector ExtendedWeaponEndPosition = FVector(0,0, 300.0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "ForeverExtended? note: pls set buff end duration to -1 to make this actually work"))
	bool ForeverExtendWeapon = false;

	virtual void OnActionBegin_Implementation() override;
	virtual void OnActionEnd_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The weapon extended buff end duration"))
	float BuffEndDuration = 0.0f;

	FTimerHandle BuffTimerHandle;

	FVector OriginalWeaponEndTracePos;
};
