// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../StructAndEnum.h"
#include "ANS_WeaponTrace.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UANS_WeaponTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual bool ShouldFireInEditor() {
		return false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BeginTrailActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanParry = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ApplyDebuff = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "ApplyDebuff", EditConditionHides, ToolTip = "Normally Use for AI"))
	FAttackDebuff DebuffToApply;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	class ABP_WeaponBase* weapon;
	class AGodsArenaCharacter* ownerCharacter;
	FCharacterCombatAnimation prevAttribute;
};
