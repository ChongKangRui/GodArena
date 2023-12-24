// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../StructAndEnum.h"
#include "ANS_SetCombatState.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UANS_SetCombatState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	//virtual 
	UPROPERTY(EditAnywhere)
		ECombatState BeginState;
	UPROPERTY(EditAnywhere)
		ECombatState EndState;
	UPROPERTY(EditAnywhere)
		ECombatType combatType = ECombatType::NormalAttack;

	void CombatContinuation(USkeletalMeshComponent* MeshComp, bool isCombatContinue);

	virtual bool ShouldFireInEditor() {
		return false;
	}
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	

	
};
