// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction_CombatAttack.h"
#include "Engine/EngineTypes.h"
#include "SAction_Ability_WeaponRangeExtend.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_Ability_WeaponRangeExtend : public USAction_CombatAttack
{
	GENERATED_BODY()
public:
	
	virtual void OnActionBegin_Implementation() override;
	virtual void OnActionEnd_Implementation() override;
	virtual void CombatResetTimer() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "In Relative position"))
	FVector ExtendedWeaponEndPosition = FVector(0, 0, 300.0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The weapon extended buff end duration"))
	float BuffEndDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "ForeverExtended? note: pls set buff end duration to -1 to make this actually work"))
	bool ForeverExtendWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Prevent buff stuck"))
	ECombatType DisableBuffCombatType;

protected:
	FTimerHandle BuffTimerHandle;
	FVector OriginalWeaponEndTracePos;
};
